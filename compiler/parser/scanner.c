#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vasq/safe_snprintf.h"

#include "scanner.h"

#define SCANNER_ERROR(format, ...) VASQ_ERROR(scanner->scanner_logger, format, ##__VA_ARGS__)

struct keywordRecord {
    const char *word;
    unsigned int len;
    int marker;
};

#define KEYWORD(literal, marker)                         \
    {                                                    \
        literal, sizeof(literal) - 1, PL_MARKER_##marker \
    }

static const struct keywordRecord keywords[] = {
    KEYWORD("true", OBJECT),
    KEYWORD("false", OBJECT),
    KEYWORD("null", OBJECT),
    KEYWORD("blank", OBJECT),
    KEYWORD("Any", TYPE),
    KEYWORD("Num", TYPE),
    KEYWORD("Int", TYPE),
    KEYWORD("Bool", TYPE),
    KEYWORD("Float", TYPE),
    KEYWORD("Array", TYPE),
    KEYWORD("GenArray", TYPE),
    KEYWORD("Bytes", TYPE),
    KEYWORD("source", SOURCE),
    KEYWORD("pipe", PIPE),
    KEYWORD("sink", SINK),
    KEYWORD("local", LOCAL),
    KEYWORD("struct", STRUCT),
    KEYWORD("opaque", OPAQUE),
    KEYWORD("while", WHILE),
    KEYWORD("if", IF),
    KEYWORD("eif", EIF),
    KEYWORD("else", ELSE),
    KEYWORD("prod", PROD),
    KEYWORD("drop", DROP),
    KEYWORD("end", END),
    KEYWORD("not", NOT),
    KEYWORD("or", LOGICAL),
    KEYWORD("and", LOGICAL),
    KEYWORD("cont", CONT),
    KEYWORD("break", BREAK),
    KEYWORD("verify", VERIFY),
    KEYWORD("abort", ABORT),
    KEYWORD("is", IS),
    KEYWORD("as", AS),
    KEYWORD("import", IMPORT),
    KEYWORD("export", EXPORT),
    KEYWORD("exportall", EXPORT_ALL),
    KEYWORD("main", MAIN),
};

#undef KEYWORD

#define CONTEXT(context)                                       \
    {                                                          \
#context, sizeof(#context) - 1, PL_SUBMARKER_##context \
    }

static const struct keywordRecord contexts[] = {
    CONTEXT(STORE),
    CONTEXT(ATTACH),
    CONTEXT(CONTEXT),
};

#undef CONTEXT

static void
advanceScanner(plLexicalScanner *scanner, unsigned int length)
{
    scanner->line += length;
    scanner->location.column_no += length;
    scanner->line_length -= length;
}

static bool
isWhitespace(char c)
{
    return (c == ' ' || c == '\t');
}

static bool
isStartingVarChar(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool
isVarChar(char c)
{
    return isStartingVarChar(c) || (c >= '0' && c <= '9');
}

static plObjectHandle
resolveStaticLiteral(char c)
{
    switch (c) {
    case 't': return PL_BOOL_LITERAL(true);
    case 'f': return PL_BOOL_LITERAL(false);
    case 'n': return PL_NULL_LITERAL;
    default: return PL_BLANK_LITERAL;
    }
}

static int
resolveType(const char *word)
{
    switch (word[0]) {
    case 'I': return PL_SUBMARKER_INT;
    case 'F': return PL_SUBMARKER_FLOAT;
    case 'N': return PL_SUBMARKER_NUM;
    case 'G': return PL_SUBMARKER_GENARRAY;
    case 'A': return (word[1] == 'n') ? PL_SUBMARKER_ANY : PL_SUBMARKER_ARRAY;
    default:  // 'B'
        return (word[1] == 'o') ? PL_SUBMARKER_BOOL : PL_SUBMARKER_BYTES;
    }
}

static int
resolveArithmetic(char c)
{
    switch (c) {
    case '+': return PL_SUBMARKER_PLUS;
    case '-': return PL_SUBMARKER_MINUS;
    case '*': return PL_SUBMARKER_MULTIPLY;
    case '/': return PL_SUBMARKER_DIVIDE;
    case '%': return PL_SUBMARKER_MODULO;
    case '|': return PL_SUBMARKER_OR;
    default: return PL_SUBMARKER_AND;  // &
    }
}

static void
consumeWhitespace(plLexicalScanner *scanner)
{
    bool stop_loop = false;

    do {
        unsigned int consumed = 0;

        if (scanner->inside_comment_block) {
            for (; consumed < scanner->line_length; consumed++) {
                if (scanner->line[consumed] == '*' && scanner->line[consumed + 1] == '/') {
                    scanner->inside_comment_block = false;
                    consumed += 2;
                    break;
                }
            }

            if (consumed == scanner->line_length) {
                stop_loop = true;
            }
        }
        else {
            for (; isWhitespace(scanner->line[consumed]); consumed++) {}

            if (scanner->line[consumed] == '/') {
                switch (scanner->line[consumed + 1]) {
                case '/': scanner->line_length = 0; return;

                case '*':
                    scanner->inside_comment_block = true;
                    consumed += 2;
                    break;

                default: stop_loop = true; break;
                }
            }
            else {
                stop_loop = true;
            }
        }

        advanceScanner(scanner, consumed);
    } while (!stop_loop);
}

static bool
prepLine(plLexicalScanner *scanner)
{
    for (consumeWhitespace(scanner); scanner->line_length == 0; consumeWhitespace(scanner)) {
        if (!fgets(scanner->buffer, sizeof(scanner->buffer), scanner->file)) {
            if (ferror(scanner->file)) {
                SCANNER_ERROR("Failed to read from %s.", scanner->file_name);
                scanner->last_marker = PL_MARKER_READ_FAILURE;
            }
            else {
                scanner->last_marker = PL_MARKER_EOF;
            }
            return false;
        }

        scanner->location.line_no++;
        scanner->location.column_no = 1;
        scanner->line_length = strnlen(scanner->buffer, sizeof(scanner->buffer));
        if (scanner->line_length >= sizeof(scanner->buffer)) {
            SCANNER_ERROR("Line too long.");
            scanner->last_marker = PL_MARKER_BAD_DATA;
            return false;
        }

        while (scanner->line_length > 0) {
            char c = scanner->buffer[scanner->line_length - 1];

            if (c == '\r' || c == '\n' || isWhitespace(c)) {
                scanner->line_length--;
            }
            else {
                break;
            }
        }

        for (unsigned int k = 0; k < scanner->line_length; k++) {
            unsigned char c = scanner->buffer[k];

            if (!isprint(c) && c != '\t') {
                SCANNER_ERROR("Unprintable character at column %u: 0x%02x", k, c);
                scanner->last_marker = PL_MARKER_BAD_DATA;
                return false;
            }
        }

        scanner->line = scanner->buffer;
        scanner->line[scanner->line_length] = '\0';

        if (scanner->line_length > 0) {
            VASQ_DEBUG(debug_logger, "%s, line %u: %.*s", scanner->file_name, scanner->location.line_no,
                       scanner->line_length, scanner->buffer);
        }
    }

    return true;
}

static int
readByteString(plLexicalScanner *scanner, plObjectHandle *handle)
{
    plByteArray *array;

    handle->as.bytes = plNewByteArray();
    if (!handle->as.bytes) {
        return PL_MARKER_OUT_OF_MEMORY;
    }
    handle->flags =
        PL_OBJ_TYPE_BYTE_ARRAY | PL_OBJ_FLAG_OWNED | PL_OBJ_FLAG_DYNAMIC | PL_OBJ_FLAG_DYNAMIC_DATA;
    array = handle->as.bytes;

    for (unsigned int k = 0; k < scanner->line_length; k++) {
        char c = scanner->line[k];

        if (c == '\\') {
            k++;
        }
        else if (c == '"') {
            array->capacity = k;
            goto good_string;
        }
    }

    PARSER_ERROR("Unterminated string literal.");
    goto error;

good_string:

    array->bytes = VASQ_MALLOC(debug_logger, array->capacity);
    if (!array->bytes) {
        plFreeObject(handle);
        return PL_MARKER_OUT_OF_MEMORY;
    }

    for (unsigned int k = 0; k < array->capacity; k++) {
        unsigned char c = scanner->line[k];

        if (c == '\\') {
            switch (scanner->line[++k]) {  // I've already checked in the above for loop that I haven't
                                           // reached the end of the string.
            case 't': c = '\t'; break;
            case 'n': c = '\n'; break;
            case 'r': c = '\r'; break;
            case '\\': break;
            case '"': c = '"'; break;
            case 'x':
                if (k + 2 >= scanner->line_length) {
                    PARSER_ERROR("Unresolved hex byte in string literal.");
                    goto error;
                }

                c = 0;
                for (int j = 0; j < 2; j++) {
                    unsigned char c2 = scanner->line[k + 1 + j], x;

                    if (!isxdigit(c2)) {
                        PARSER_ERROR("Invalid hex byte in string literal.");
                        goto error;
                    }

                    if (isdigit(c2)) {
                        x = c2 - '0';
                    }
                    else if (islower(c2)) {
                        x = 10 + c2 - 'a';
                    }
                    else {
                        x = 10 + c2 - 'A';
                    }

                    c |= (x << (4 - 4 * j));
                }
                k += 2;
                break;

            default:
                PARSER_ERROR("Invalid escaped character in string literal: %c", scanner->line[k]);
                goto error;
            }
        }

        array->bytes[array->length++] = c;
    }

    advanceScanner(scanner, array->capacity + 1);  // The +1 is for the ending double quotes.

    return PL_MARKER_OBJECT;

error:

    plFreeObject(handle);
    return PL_MARKER_BAD_DATA;
}

static void
lookaheadStoreLogic(plLexicalScanner *scanner, const plLexicalToken *token)
{
    if (scanner->num_look_ahead > 0) {
        unsigned int num_to_move;

        num_to_move = MIN(scanner->num_look_ahead, PL_SCANNER_MAX_LOOK_AHEAD - 1);
        memmove(scanner->look_ahead + 1, scanner->look_ahead + 0, sizeof(*token) * num_to_move);
    }
    memcpy(scanner->look_ahead + 0, token, sizeof(*token));
    scanner->num_look_ahead++;
}

static void
scannerProcessor(void *user_data, size_t position, vasqLogLevel_t level, char **dst, size_t *remaining)
{
    (void)position;
    (void)level;
    const plLexicalScanner *scanner = user_data;

    vasqIncSnprintf(dst, remaining, "%s:%u", scanner->file_name, scanner->location.line_no);
}

static const char *
stripLineBeginning(const char *line)
{
    while (isWhitespace(*line)) {
        line++;
    }

    return line;
}

static void
parserProcessor(void *user_data, size_t position, vasqLogLevel_t level, char **dst, size_t *remaining)
{
    const plLexicalScanner *scanner = user_data;
    plLexicalLocation location;

    plGetLastLocation(scanner, &location);

    switch (position) {
        const char *error_string;

    case 0:
        error_string = (level == VASQ_LL_WARNING) ? WARNING_STRING : ERROR_STRING;
        vasqIncSnprintf(dst, remaining, "%s%s:%u:%u", error_string, scanner->file_name, location.line_no,
                        location.column_no);
        break;

    case 1: vasqIncSnprintf(dst, remaining, "%s", stripLineBeginning(scanner->buffer)); break;

    case 2:
        for (unsigned int k = 1; k < location.column_no; k++) {
            vasqIncSnprintf(dst, remaining, " ");
        }
        vasqIncSnprintf(dst, remaining, "^");
        break;

    default: break;
    }
}

int
plScannerInit(plLexicalScanner *scanner, FILE *file, const char *file_name)
{
    int ret;
    vasqLoggerOptions options;

    if (!scanner || !file) {
        VASQ_ERROR(debug_logger, "scanner and file cannot be NULL.");
        return PL_MARKER_USAGE;
    }

    *scanner = (plLexicalScanner){0};

    scanner->table = plWordTableNew();
    if (!scanner->table) {
        return PL_RET_OUT_OF_MEMORY;
    }

    scanner->keyword_table = plRefTableNew();
    if (!scanner->keyword_table) {
        ret = PL_RET_OUT_OF_MEMORY;
        goto error;
    }

    for (size_t k = 0; k < ARRAY_LENGTH(keywords); k++) {
        if (!plUpdateRef(scanner->keyword_table, keywords[k].word, (void *)(intptr_t)keywords[k].marker)) {
            ret = PL_RET_OUT_OF_MEMORY;
            goto error;
        }
    }

    options.flags = 0;
    options.processor = scannerProcessor;
    options.user = scanner;
    ret = vasqLoggerCreate(STDOUT_FILENO, VASQ_LL_WARNING, PL_LOGGER_PREAMBLE "%x: %M\n", &options,
                           &scanner->scanner_logger);
    if (ret != VASQ_RET_OK) {
        goto error;
    }

    options.processor = parserProcessor;
    ret = vasqLoggerCreate(STDOUT_FILENO, VASQ_LL_WARNING, PL_LOGGER_PREAMBLE "%x: %M\n\t%x\n\t%x\n",
                           &options, &scanner->parser_logger);
    if (ret != VASQ_RET_OK) {
        goto error;
    }

    scanner->file = file;
    scanner->file_name = file_name ? file_name : "<anonymous file>";
    scanner->last_marker = PL_MARKER_INIT;
    scanner->line = scanner->buffer;

    return PL_RET_OK;

error:

    plScannerCleanup(scanner);

    return plTranslateVasqRet(ret);
}

void
plScannerCleanup(plLexicalScanner *scanner)
{
    if (scanner) {
        unsigned int num_look_ahead = scanner->num_look_ahead;

        for (unsigned int k = 0; k < num_look_ahead; k++) {
            plTokenCleanup(scanner->look_ahead + k, scanner->table);
        }

        vasqLoggerFree(scanner->scanner_logger);
        vasqLoggerFree(scanner->parser_logger);
        plWordTableFree(scanner->table);
        plRefTableFree(scanner->keyword_table);

        *scanner = (plLexicalScanner){0};
    }
}

int
plTokenRead(plLexicalScanner *scanner, plLexicalToken *token)
{
    unsigned int consumed;

    if (!scanner || !token) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_MARKER_USAGE;
    }

    if (TERMINAL_MARKER(scanner->last_marker)) {
        goto return_marker;
    }

    if (scanner->num_look_ahead > 0) {
        memcpy(token, scanner->look_ahead + 0, sizeof(*token));
        scanner->last_marker = token->header.marker;
        memcpy(&scanner->last_look_ahead_loc, &token->header.location, sizeof(token->header.location));
        if (scanner->num_look_ahead > 1) {
            memmove(scanner->look_ahead + 0, scanner->look_ahead + 1,
                    sizeof(*token) * (scanner->num_look_ahead - 1));
        }
        scanner->num_look_ahead--;
        goto return_marker;
    }
    else {
        scanner->last_look_ahead_loc.line_no = 0;
    }

    if (!prepLine(scanner)) {
        goto return_marker;
    }

    token->data.handle = (plObjectHandle){0};
    token->header.submarker = PL_SUBMARKER_NONE;
    memcpy(&token->header.location, &scanner->location, sizeof(scanner->location));
    consumed = 1;

    switch (scanner->line[0]) {
    case ';': scanner->last_marker = PL_MARKER_SEMICOLON; goto done;

    case ':': scanner->last_marker = PL_MARKER_COLON; goto done;

    case ',': scanner->last_marker = PL_MARKER_COMMA; goto done;

    case '.': scanner->last_marker = PL_MARKER_PERIOD; goto done;

    case '(': scanner->last_marker = PL_MARKER_LEFT_PARENS; goto done;

    case ')': scanner->last_marker = PL_MARKER_RIGHT_PARENS; goto done;

    case '{': scanner->last_marker = PL_MARKER_LEFT_BRACE; goto done;

    case '}': scanner->last_marker = PL_MARKER_RIGHT_BRACE; goto done;

    case '[': scanner->last_marker = PL_MARKER_LEFT_BRACKET; goto done;

    case ']': scanner->last_marker = PL_MARKER_RIGHT_BRACKET; goto done;

    case '-':
        if (scanner->line[1] == '>') {
            scanner->last_marker = PL_MARKER_ARROW;
            consumed = 2;
            goto done;
        }
        /* FALLTHROUGH */
    case '+':
    case '*':
    case '/':
    case '%':
    case '|':
    case '&':
        token->header.submarker = resolveArithmetic(scanner->line[0]);
        if (scanner->line[1] == '=') {
            scanner->last_marker = PL_MARKER_REASSIGNMENT;
            consumed = 2;
        }
        else {
            scanner->last_marker = PL_MARKER_ARITHMETIC;
        }

        goto done;

    case '=':
    case '!':
        if (scanner->line[1] == '=') {
            scanner->last_marker = PL_MARKER_COMPARISON;
            token->header.submarker =
                (scanner->line[0] == '=') ? PL_SUBMARKER_EQUAL : PL_SUBMARKER_NOT_EQUAL;
            consumed = 2;
            goto done;
        }
        else {
            PARSER_ERROR("Invalid token: '%c'", scanner->line[0]);
            scanner->last_marker = PL_MARKER_BAD_DATA;
            goto return_marker;
        }

    case '<':
    case '>':
        if (scanner->line[1] == scanner->line[0]) {
            if (scanner->line[2] == '=') {
                scanner->last_marker = PL_MARKER_REASSIGNMENT;
                consumed = 3;
            }
            else {
                scanner->last_marker = PL_MARKER_ARITHMETIC;
                consumed = 2;
            }

            token->header.submarker = (scanner->line[0] == '<') ? PL_SUBMARKER_LSHIFT : PL_SUBMARKER_RSHIFT;
        }
        else {
            if (scanner->line[1] == '=') {
                token->header.submarker =
                    (scanner->line[0] == '<') ? PL_SUBMARKER_LESS_THAN_EQ : PL_SUBMARKER_GREATER_THAN_EQ;
                consumed = 2;
            }
            else {
                token->header.submarker =
                    (scanner->line[0] == '<') ? PL_SUBMARKER_LESS_THAN : PL_SUBMARKER_GREATER_THAN;
            }

            scanner->last_marker = PL_MARKER_COMPARISON;
        }
        goto done;

    case '?':
        if (isVarChar(scanner->line[1])) {
            unsigned int end;

            for (size_t k = 0; k < ARRAY_LENGTH(contexts); k++) {
                if (strncmp(scanner->line + 1, contexts[k].word, contexts[k].len) == 0) {
                    scanner->last_marker = PL_MARKER_CONTEXT;
                    token->header.submarker = contexts[k].marker;
                    consumed = 1 + contexts[k].len;
                    goto done;
                }
            }

            for (end = 2; isVarChar(scanner->line[end]); end++) {}
            PARSER_ERROR("Invalid context: %.*s", end - 1, scanner->line + 1);
            scanner->last_marker = PL_MARKER_BAD_DATA;
            goto return_marker;
        }
        else {
            scanner->last_marker = PL_MARKER_QUESTION;
            goto done;
        }

    default: break;
    }

    if (isStartingVarChar(scanner->line[0])) {
        void *ref;

        for (consumed = 1; consumed < scanner->line_length && isVarChar(scanner->line[consumed]);
             consumed++) {}

        if (consumed + 1 < scanner->line_length && scanner->line[consumed] == '?' &&
            isVarChar(scanner->line[consumed + 1])) {
            PARSER_ERROR("A '?' cannot connect two variable characters");
            scanner->last_marker = PL_MARKER_BAD_DATA;
            goto return_marker;
        }

        if (plLookupRef(scanner->keyword_table, scanner->line, consumed, &ref)) {
            scanner->last_marker = (int)(intptr_t)ref;

            switch (scanner->last_marker) {
            case PL_MARKER_OBJECT: token->data.handle = resolveStaticLiteral(scanner->line[0]); break;
            case PL_MARKER_LOGICAL:
                token->header.submarker = (scanner->line[0] == 'o') ? PL_SUBMARKER_OR : PL_SUBMARKER_AND;
                break;
            case PL_MARKER_TYPE: token->header.submarker = resolveType(scanner->line); break;
            default: break;
            }
        }
        else if (consumed == 1 && scanner->line[0] == '_') {
            scanner->last_marker = PL_MARKER_UNDERSCORE;
        }
        else {
            token->data.name = plRegisterWord(scanner->table, scanner->line, consumed);
            if (!token->data.name) {
                scanner->last_marker = PL_MARKER_OUT_OF_MEMORY;
                goto return_marker;
            }

            scanner->last_marker = PL_MARKER_NAME;
        }
    }
    else if (scanner->line[0] == '0' && scanner->line[1] == 'x') {
        for (consumed = 0; consumed < scanner->line_length; consumed++) {
            if (!isxdigit(scanner->line[consumed])) {
                if (isVarChar(scanner->line[consumed])) {
                    PARSER_ERROR("Invalid hex literal.");
                    scanner->last_marker = PL_MARKER_BAD_DATA;
                    goto return_marker;
                }

                break;
            }
        }

        if (plPopulateIntegerFromHexString(scanner->line + 2, consumed, &token->data.handle.as.integer) !=
            PL_RET_OK) {
            scanner->last_marker = PL_MARKER_OUT_OF_MEMORY;
            goto return_marker;
        }
        consumed += 2;
        token->data.handle.flags = PL_OBJ_TYPE_INT | PL_OBJ_FLAG_OWNED;
    }
    else if (isdigit(scanner->line[0])) {
        int value;

        for (consumed = 1; consumed < scanner->line_length && isdigit(scanner->line[consumed]); consumed++) {
        }

        if (isVarChar(scanner->line[consumed])) {
            PARSER_ERROR("Invalid numeric literal.");
            scanner->last_marker = PL_MARKER_BAD_DATA;
            goto return_marker;
        }

        if (scanner->line[consumed] == '.') {
            for (consumed++; consumed < scanner->line_length && isdigit(scanner->line[consumed]);
                 consumed++) {}

            if (isVarChar(scanner->line[consumed])) {
                PARSER_ERROR("Invalid numeric literal.");
                scanner->last_marker = PL_MARKER_BAD_DATA;
                goto return_marker;
            }

            token->data.handle.flags = PL_OBJ_TYPE_FLOAT | PL_OBJ_FLAG_OWNED;
            value = plPopulateFloatFromString(scanner->line, consumed, &token->data.handle.as.decimal);
        }
        else {
            token->data.handle.flags = PL_OBJ_TYPE_INT | PL_OBJ_FLAG_OWNED;
            value = plPopulateIntegerFromString(scanner->line, consumed, &token->data.handle.as.integer);
        }

        if (value != PL_RET_OK) {
            if (value == PL_RET_OUT_OF_MEMORY) {
                scanner->last_marker = PL_MARKER_OUT_OF_MEMORY;
            }
            else {
                PARSER_ERROR("Invalid numeric literal.");
                scanner->last_marker = PL_MARKER_BAD_DATA;
            }
            goto return_marker;
        }

        scanner->last_marker = PL_MARKER_OBJECT;
    }
    else if (isVarChar(scanner->line[0])) {
        for (; consumed < scanner->line_length && isVarChar(scanner->line[consumed]); consumed++) {}

        if (consumed == 1 && scanner->line[0] == '_') {
            scanner->last_marker = PL_MARKER_UNDERSCORE;
        }
        else {
            token->data.name = plRegisterWord(scanner->table, scanner->line, consumed);
            if (!token->data.name) {
                scanner->last_marker = PL_MARKER_OUT_OF_MEMORY;
                goto return_marker;
            }

            scanner->last_marker = PL_MARKER_NAME;
        }
    }
    else if (scanner->line[0] == '"') {
        advanceScanner(scanner, 1);
        scanner->last_marker = readByteString(scanner, &token->data.handle);
        if (TERMINAL_MARKER(scanner->last_marker)) {
            goto return_marker;
        }
        consumed = 0;  // readByteString already advanced the scanner.
    }
    else {
        PARSER_ERROR("Invalid token: '%c'", scanner->line[0]);
        scanner->last_marker = PL_MARKER_BAD_DATA;
        goto return_marker;
    }

done:

    token->header.marker = scanner->last_marker;
    advanceScanner(scanner, consumed);

return_marker:

    return scanner->last_marker;
}

int
plTranslateTerminalMarker(int marker)
{
    switch (marker) {
    case PL_MARKER_USAGE: return PL_RET_USAGE;
    case PL_MARKER_READ_FAILURE: return PL_RET_IO;
    case PL_MARKER_OUT_OF_MEMORY: return PL_RET_OUT_OF_MEMORY;
    default: return PL_RET_BAD_DATA;
    }
}

void
plGetLastLocation(const plLexicalScanner *scanner, plLexicalLocation *location)
{
    const plLexicalLocation *ptr;

    if (!scanner || !location) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return;
    }

    if (scanner->last_look_ahead_loc.line_no > 0) {
        ptr = &scanner->last_look_ahead_loc;
    }
    else {
        ptr = &scanner->location;
    }

    memcpy(location, ptr, sizeof(*ptr));
}

void
plTokenCleanup(plLexicalToken *token, plWordTable *table)
{
    if (!token) {
        return;
    }

    switch (token->header.marker) {
    case PL_MARKER_NAME:
        plUnregisterWord(table, token->data.name);  // plUnregisterWord will check to see if table is NULL.
        break;

    case PL_MARKER_OBJECT: plFreeObject(&token->data.handle); break;

    default: break;
    }
}

#if LL_USE == -1

void
plLookaheadStoreNoLog(plLexicalScanner *scanner, plLexicalToken *token)
{
    if (!scanner || !token) {
        return;
    }

    assert(scanner->num_look_ahead < PL_SCANNER_MAX_LOOK_AHEAD);

    lookaheadStoreLogic(scanner, token);
}

#else  // LL_USE == -1

int
plTokenReadLog(const char *file_name, const char *function_name, unsigned int line_no,
               plLexicalScanner *scanner, plLexicalToken *token)
{
    int ret;

    ret = plTokenRead(scanner, token);
    if (!TERMINAL_MARKER(ret)) {
        vasqLogStatement(debug_logger, VASQ_LL_INFO, file_name, function_name, line_no, "Read token: %s",
                         plLexicalMarkerName(ret));
    }
    else if (ret == PL_MARKER_EOF) {
        vasqLogStatement(debug_logger, VASQ_LL_INFO, file_name, function_name, line_no,
                         "End of file reached.");
    }
    return ret;
}

void
plLookaheadStoreLog(const char *file_name, const char *function_name, unsigned int line_no,
                    plLexicalScanner *scanner, plLexicalToken *token)
{
    if (!scanner || !token) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return;
    }

    VASQ_ASSERT(debug_logger, scanner->num_look_ahead < PL_SCANNER_MAX_LOOK_AHEAD);

    lookaheadStoreLogic(scanner, token);
    vasqLogStatement(debug_logger, VASQ_LL_INFO, file_name, function_name, line_no,
                     "%s stored as look ahead (%u total).", plLexicalMarkerName(token->header.marker),
                     scanner->num_look_ahead);
}

#endif  // LL_USE == -1
