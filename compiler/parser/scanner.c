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
    KEYWORD("exit", EXIT),
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
            while (isWhitespace(scanner->line[consumed])) {
                consumed++;
            }

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

static void
clearStored(plLexicalScanner *scanner)
{
    for (unsigned int k = 0; k < scanner->num_stored; k++) {
        plTokenCleanup(&scanner->store[k], scanner->table);
    }
}

static int
setError(plLexicalScanner *scanner, int marker)
{
    clearStored(scanner);
    PEEK_TOKEN(scanner, 0) = marker;
    scanner->num_stored = 1;
    return plTranslateTerminalMarker(marker);
}

static bool
prepLine(plLexicalScanner *scanner)
{
    for (consumeWhitespace(scanner); scanner->line_length == 0; consumeWhitespace(scanner)) {
        if (!fgets(scanner->buffer, sizeof(scanner->buffer), scanner->file)) {
            if (ferror(scanner->file)) {
                SCANNER_ERROR("Failed to read from %s.", scanner->file_name);
                setError(scanner, PL_MARKER_READ_FAILURE);
                return false;
            }
            return true;
        }

        scanner->location.line_no++;
        scanner->location.column_no = 1;
        scanner->line_length = strnlen(scanner->buffer, sizeof(scanner->buffer));
        if (scanner->line_length >= sizeof(scanner->buffer)) {
            SCANNER_ERROR("Line too long.");
            setError(scanner, PL_MARKER_BAD_DATA);
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
                setError(scanner, PL_MARKER_BAD_DATA);
                return false;
            }
        }

        scanner->line = scanner->buffer;
        scanner->line[scanner->line_length] = '\0';

        if (scanner->line_length > 0) {
            VASQ_DEBUG(debug_logger, "%s, line %u: %.*s", scanner->file_name, scanner->location.line_no,
                       scanner->line_length, scanner->buffer);
            plRegisterLine(scanner->line_table, scanner->location.line_no, scanner->buffer,
                           scanner->line_length);
        }
    }

    return true;
}

static bool
readByteString(plLexicalScanner *scanner, plObjectHandle *handle)
{
    plByteArray *array;

    handle->as.bytes = plNewByteArray();
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

    array->bytes = plSafeMalloc(array->capacity);

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

    return true;

error:

    plFreeObject(handle);
    return false;
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
    plLexicalScanner *scanner = user_data;
    plLexicalLocation *location;

    if (scanner->error_on_peek < 0) {
        location = &scanner->last_consumed_location;
    }
    else {
        location = &scanner->store[(unsigned int)scanner->error_on_peek].header.location;
    }

    switch (position) {
        const char *error_string, *line, *stripped_line;

    case 0:
        error_string = (level == VASQ_LL_WARNING) ? WARNING_STRING : ERROR_STRING;
        vasqIncSnprintf(dst, remaining, "%s%s:%u:%u", error_string, scanner->file_name, location->line_no,
                        location->column_no);
        break;

    case 1:
        line = plLookupLine(scanner->line_table, location->line_no);
        stripped_line = stripLineBeginning(line);
        scanner->whitespace_stripped = (uintptr_t)stripped_line - (uintptr_t)line;
        vasqIncSnprintf(dst, remaining, "%s", stripped_line);
        break;

    case 2:
        for (unsigned int k = 1 + scanner->whitespace_stripped; k < location->column_no; k++) {
            vasqIncSnprintf(dst, remaining, " ");
        }
        vasqIncSnprintf(dst, remaining, "^");
        break;

    default: break;
    }
}

static int
tokenRead(plLexicalScanner *scanner)
{
    int marker;
    unsigned int consumed;
    plLexicalToken *token;

    if (UNLIKELY(!scanner)) {
        VASQ_ERROR(debug_logger, "scanner cannot be NULL.");
        return PL_RET_USAGE;
    }

    if (UNLIKELY(scanner->num_stored == PL_SCANNER_MAX_STORE)) {
        VASQ_ERROR(debug_logger, "The token store is full.");
        return PL_RET_USAGE;
    }

    if (scanner->num_stored > 0 && scanner->store[scanner->num_stored - 1].header.marker == PL_MARKER_EOF) {
        return PL_RET_OK;
    }

    if (TERMINAL_MARKER(PEEK_TOKEN(scanner, 0)) || !prepLine(scanner)) {
        return plTranslateTerminalMarker(PEEK_TOKEN(scanner, 0));
    }

    token = &scanner->store[scanner->num_stored];
    token->data.handle = (plObjectHandle){0};
    token->header.submarker = PL_SUBMARKER_NONE;
    memcpy(&token->header.location, &scanner->location, sizeof(scanner->location));
    consumed = 1;

    switch (scanner->line[0]) {
    case '\0': marker = PL_MARKER_EOF; goto done;

    case ';': marker = PL_MARKER_SEMICOLON; goto done;

    case ':': marker = PL_MARKER_COLON; goto done;

    case ',': marker = PL_MARKER_COMMA; goto done;

    case '.': marker = PL_MARKER_PERIOD; goto done;

    case '(': marker = PL_MARKER_LEFT_PARENS; goto done;

    case ')': marker = PL_MARKER_RIGHT_PARENS; goto done;

    case '{': marker = PL_MARKER_LEFT_BRACE; goto done;

    case '}': marker = PL_MARKER_RIGHT_BRACE; goto done;

    case '[': marker = PL_MARKER_LEFT_BRACKET; goto done;

    case ']': marker = PL_MARKER_RIGHT_BRACKET; goto done;

    case '-':
        if (scanner->line[1] == '>') {
            marker = PL_MARKER_ARROW;
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
            marker = PL_MARKER_REASSIGNMENT;
            consumed = 2;
        }
        else {
            marker = PL_MARKER_ARITHMETIC;
        }

        goto done;

    case '=':
    case '!':
        if (scanner->line[1] == '=') {
            marker = PL_MARKER_COMPARISON;
            token->header.submarker =
                (scanner->line[0] == '=') ? PL_SUBMARKER_EQUAL : PL_SUBMARKER_NOT_EQUAL;
            consumed = 2;
            goto done;
        }
        else {
            PARSER_ERROR("Invalid token: '%c'", scanner->line[0]);
            return setError(scanner, PL_MARKER_BAD_DATA);
        }

    case '<':
    case '>':
        if (scanner->line[1] == scanner->line[0]) {
            if (scanner->line[2] == '=') {
                marker = PL_MARKER_REASSIGNMENT;
                consumed = 3;
            }
            else {
                marker = PL_MARKER_ARITHMETIC;
                consumed = 2;
            }

            token->header.submarker = (scanner->line[0] == '<') ? PL_SUBMARKER_LSHIFT : PL_SUBMARKER_RSHIFT;
        }
        else {
            marker = PL_MARKER_COMPARISON;

            if (scanner->line[1] == '=') {
                token->header.submarker =
                    (scanner->line[0] == '<') ? PL_SUBMARKER_LESS_THAN_EQ : PL_SUBMARKER_GREATER_THAN_EQ;
                consumed = 2;
            }
            else {
                token->header.submarker =
                    (scanner->line[0] == '<') ? PL_SUBMARKER_LESS_THAN : PL_SUBMARKER_GREATER_THAN;
            }
        }
        goto done;

    case '?':
        if (isVarChar(scanner->line[1])) {
            unsigned int len;
            char *start = scanner->line + 1;

            for (len = 1; isVarChar(start[len]); len++) {}

            for (size_t k = 0; k < ARRAY_LENGTH(contexts); k++) {
                if (contexts[k].len == len && strncmp(start, contexts[k].word, len) == 0) {
                    marker = PL_MARKER_CONTEXT;
                    token->header.submarker = contexts[k].marker;
                    consumed = 1 + len;
                    goto done;
                }
            }

            PARSER_ERROR("Invalid context: %.*s", len, start);
            return setError(scanner, PL_MARKER_BAD_DATA);
        }
        else {
            marker = PL_MARKER_QUESTION;
            goto done;
        }

    default: break;
    }

    if (isStartingVarChar(scanner->line[0])) {
        void *ref;

        for (; consumed < scanner->line_length && isVarChar(scanner->line[consumed]); consumed++) {}

        if (consumed + 1 < scanner->line_length && scanner->line[consumed] == '?' &&
            isVarChar(scanner->line[consumed + 1])) {
            PARSER_ERROR("A '?' cannot connect two variable characters.");
            return setError(scanner, PL_MARKER_BAD_DATA);
        }

        if (plLookupRef(scanner->keyword_table, scanner->line, consumed, &ref)) {
            marker = (int)(intptr_t)ref;

            switch (marker) {
            case PL_MARKER_OBJECT: token->data.handle = resolveStaticLiteral(scanner->line[0]); break;
            case PL_MARKER_LOGICAL:
                token->header.submarker = (scanner->line[0] == 'o') ? PL_SUBMARKER_OR : PL_SUBMARKER_AND;
                break;
            case PL_MARKER_TYPE: token->header.submarker = resolveType(scanner->line); break;
            default: break;
            }
        }
        else if (consumed == 1 && scanner->line[0] == '_') {
            marker = PL_MARKER_UNDERSCORE;
        }
        else {
            token->data.name = plRegisterWord(scanner->table, scanner->line, consumed);
            if (!token->data.name) {
                return setError(scanner, PL_MARKER_OUT_OF_MEMORY);
            }

            marker = PL_MARKER_NAME;
        }
    }
    else if (scanner->line[0] == '0' && scanner->line[1] == 'x') {
        for (consumed = 0; consumed < scanner->line_length; consumed++) {
            if (!isxdigit(scanner->line[consumed])) {
                if (isVarChar(scanner->line[consumed])) {
                    PARSER_ERROR("Invalid hex literal.");
                    return setError(scanner, PL_MARKER_BAD_DATA);
                }

                break;
            }
        }

        if (plPopulateIntegerFromHexString(scanner->line + 2, consumed, &token->data.handle.as.integer) !=
            PL_RET_OK) {
            return setError(scanner, PL_MARKER_OUT_OF_MEMORY);
        }
        consumed += 2;
        token->data.handle.flags = PL_OBJ_TYPE_INT | PL_OBJ_FLAG_OWNED;
        marker = PL_MARKER_OBJECT;
    }
    else if (isdigit(scanner->line[0])) {
        int value;

        for (; consumed < scanner->line_length && isdigit(scanner->line[consumed]); consumed++) {}

        if (isVarChar(scanner->line[consumed])) {
            PARSER_ERROR("Invalid numeric literal.");
            return setError(scanner, PL_MARKER_BAD_DATA);
        }

        if (scanner->line[consumed] == '.') {
            for (consumed++; consumed < scanner->line_length && isdigit(scanner->line[consumed]);
                 consumed++) {}

            if (isVarChar(scanner->line[consumed])) {
                PARSER_ERROR("Invalid numeric literal.");
                return setError(scanner, PL_MARKER_BAD_DATA);
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
                marker = PL_MARKER_OUT_OF_MEMORY;
            }
            else {
                PARSER_ERROR("Invalid numeric literal.");
                marker = PL_MARKER_BAD_DATA;
            }
            return setError(scanner, marker);
        }

        marker = PL_MARKER_OBJECT;
    }
    else if (isVarChar(scanner->line[0])) {
        for (; consumed < scanner->line_length && isVarChar(scanner->line[consumed]); consumed++) {}

        if (consumed == 1 && scanner->line[0] == '_') {
            marker = PL_MARKER_UNDERSCORE;
        }
        else {
            token->data.name = plRegisterWord(scanner->table, scanner->line, consumed);
            if (!token->data.name) {
                return setError(scanner, PL_MARKER_OUT_OF_MEMORY);
            }
            marker = PL_MARKER_NAME;
        }
    }
    else if (scanner->line[0] == '"') {
        advanceScanner(scanner, 1);
        if (!readByteString(scanner, &token->data.handle)) {
            return setError(scanner, PL_MARKER_BAD_DATA);
        }
        consumed = 0;  // readByteString already advanced the scanner.
        marker = PL_MARKER_OBJECT;
    }
    else {
        PARSER_ERROR("Invalid token: '%c'", scanner->line[0]);
        return setError(scanner, PL_MARKER_BAD_DATA);
    }

done:

    token->header.marker = marker;
    scanner->num_stored++;
    advanceScanner(scanner, consumed);
    return PL_RET_OK;
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
    scanner->keyword_table = plRefTableNew();
    scanner->line_table = plLineTableNew();

    for (size_t k = 0; k < ARRAY_LENGTH(keywords); k++) {
        plUpdateRef(scanner->keyword_table, keywords[k].word, (void *)(intptr_t)keywords[k].marker);
    }

    options.flags = 0;
    options.processor = scannerProcessor;
    options.user = scanner;
    ret = vasqLoggerCreate(STDOUT_FILENO, VASQ_LL_WARNING, PL_LOGGER_PREAMBLE "%x: %M\n", &options,
                           &scanner->scanner_logger);
    if (UNLIKELY(ret != VASQ_RET_OK)) {
        goto error;
    }

    options.processor = parserProcessor;
    ret = vasqLoggerCreate(STDOUT_FILENO, VASQ_LL_WARNING, PL_LOGGER_PREAMBLE "%x: %M\n\t%x\n\t%x\n",
                           &options, &scanner->parser_logger);
    if (UNLIKELY(ret != VASQ_RET_OK)) {
        goto error;
    }

    scanner->file = file;
    scanner->file_name = file_name ? file_name : "<anonymous file>";
    PEEK_TOKEN(scanner, 0) = PL_MARKER_INIT;
    scanner->error_on_peek = -1;
    scanner->line = scanner->buffer;

    for (unsigned int k = 0; k < PL_SCANNER_MAX_STORE; k++) {
        ret = tokenRead(scanner);
        if (ret != PL_RET_OK) {
            goto error;
        }
    }

    return PL_RET_OK;

error:

    plScannerCleanup(scanner);

    return plTranslateVasqRet(ret);
}

void
plScannerCleanup(plLexicalScanner *scanner)
{
    if (!scanner) {
        return;
    }

    clearStored(scanner);

    vasqLoggerFree(scanner->scanner_logger);
    vasqLoggerFree(scanner->parser_logger);
    plWordTableFree(scanner->table);
    plRefTableFree(scanner->keyword_table);
    plLineTableFree(scanner->line_table);

    *scanner = (plLexicalScanner){0};
}

int
plTokenConsume(plLexicalScanner *scanner, plLexicalToken *token)
{
    int ret;

    if (UNLIKELY(!scanner)) {
        VASQ_ERROR(debug_logger, "scanner cannot be NULL.");
        return PL_RET_USAGE;
    }

    if (UNLIKELY(scanner->num_stored == 0 || scanner->num_stored > PL_SCANNER_MAX_STORE)) {
        VASQ_CRITICAL(debug_logger, "scanner->num_stored is invalid (%u).", scanner->num_stored);
        return PL_RET_USAGE;
    }

    if (TERMINAL_MARKER(PEEK_TOKEN(scanner, 0)) && PEEK_TOKEN(scanner, 0) != PL_MARKER_EOF) {
        return plTranslateTerminalMarker(PEEK_TOKEN(scanner, 0));
    }

    if (token) {
        memcpy(token, &scanner->store[0], sizeof(*token));
    }
    else if (UNLIKELY(CONTAINS_DATA(PEEK_TOKEN(scanner, 0)))) {
        VASQ_CRITICAL(debug_logger, "Cannot throw away a token which contains data (%s)",
                      plLexicalMarkerName(PEEK_TOKEN(scanner, 0)));
        return PL_RET_USAGE;
    }

    memcpy(&scanner->last_consumed_location, &scanner->store[0].header.location,
           sizeof(scanner->last_consumed_location));

    if (scanner->num_stored > 1) {  // Otherwise, the last marker must be EOF.
        memmove(&scanner->store[0], &scanner->store[1], sizeof(*token) * --scanner->num_stored);
    }

    ret = tokenRead(scanner);
    if (ret != PL_RET_OK && token) {
        plTokenCleanup(token, scanner->table);
    }
    return ret;
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

#if LL_USE != -1

int
plTokenConsumeLog(const char *file_name, const char *function_name, unsigned int line_no,
                  plLexicalScanner *scanner, plLexicalToken *token)
{
    int ret, marker;
    plLexicalToken temp_token;
    plLexicalToken *token_ptr = token ? token : &temp_token;

    ret = plTokenConsume(scanner, token_ptr);
    if (ret != PL_RET_OK) {
        return ret;
    }
    marker = token_ptr->header.marker;
    if (!TERMINAL_MARKER(marker)) {
        vasqLogStatement(debug_logger, VASQ_LL_INFO, file_name, function_name, line_no, "Consumed token: %s",
                         plLexicalMarkerName(marker));
    }
    else if (marker == PL_MARKER_EOF) {
        vasqLogStatement(debug_logger, VASQ_LL_INFO, file_name, function_name, line_no,
                         "End of file reached");
    }
    return PL_RET_OK;
}

#endif  // LL_USE != -1

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
