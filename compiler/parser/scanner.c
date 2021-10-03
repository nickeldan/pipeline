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

static const struct keywordRecord keywords[] = {
    {"true", 4, PL_MARKER_OBJECT},
    {"false", 4, PL_MARKER_OBJECT},
    {"null", 4, PL_MARKER_OBJECT},
    {"blank", 5, PL_MARKER_OBJECT},
    {"Any", 3, PL_MARKER_TYPE},
    {"Num", 3, PL_MARKER_TYPE},
    {"Int", 3, PL_MARKER_TYPE},
    {"Bool", 4, PL_MARKER_TYPE},
    {"Float", 5, PL_MARKER_TYPE},
    {"Array", 5, PL_MARKER_TYPE},
    {"GenArray", 8, PL_MARKER_TYPE},
    {"Bytes", 5, PL_MARKER_TYPE},
    {"source", 6, PL_MARKER_SOURCE},
    {"pipe", 4, PL_MARKER_PIPE},
    {"sink", 4, PL_MARKER_SINK},
    {"local", 5, PL_MARKER_LOCAL},
    {"struct", 6, PL_MARKER_STRUCT},
    {"typedecl", 8, PL_MARKER_TYPE_DECL},
    {"while", 5, PL_MARKER_WHILE},
    {"if", 2, PL_MARKER_IF},
    {"eif", 3, PL_MARKER_EIF},
    {"else", 4, PL_MARKER_ELSE},
    {"prod", 4, PL_MARKER_PROD},
    {"drop", 4, PL_MARKER_DROP},
    {"end", 3, PL_MARKER_END},
    {"not", 3, PL_MARKER_NOT},
    {"or", 2, PL_MARKER_LOGICAL},
    {"and", 3, PL_MARKER_LOGICAL},
    {"cont", 4, PL_MARKER_CONT},
    {"break", 5, PL_MARKER_BREAK},
    {"verify", 6, PL_MARKER_VERIFY},
    {"abort", 5, PL_MARKER_ABORT},
    {"is", 2, PL_MARKER_IS},
    {"as", 2, PL_MARKER_AS},
    {"import", 6, PL_MARKER_IMPORT},
    {"export", 6, PL_MARKER_EXPORT},
    {"exportall", 9, PL_MARKER_EXPORT_ALL},
    {"main", 4, PL_MARKER_MAIN},
};

static const struct keywordRecord contexts[] = {
    {"STORE", 5, PL_SUBMARKER_STORE},
    {"ATTACH", 6, PL_SUBMARKER_ATTACH},
    {"CONTEXT", 7, PL_SUBMARKER_CONTEXT},
};

static bool
isWhitespace(char c)
{
    return (c == ' ' || c == '\t');
}

static bool
isVarChar(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
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
advanceScanner(plLexicalScanner *scanner, unsigned int length)
{
    scanner->line += length;
    scanner->location.column_no += length;
    scanner->line_length -= length;
}

#if LL_USE == -1

#define ADVANCE_SCANNER(scanner, length) advanceScanner(scanner, length)

#else

static void
advanceScannerLog(const char *function_name, unsigned int line_no, plLexicalScanner *scanner,
                  unsigned int length)
{
    advanceScanner(scanner, length);
    if (length > 0) {
        vasqLogStatement(debug_logger, VASQ_LL_DEBUG, __FILE__, function_name, line_no,
                         "%u character%s consumed.", length, (length == 1) ? "" : "s");
    }
}
#define ADVANCE_SCANNER(scanner, length) advanceScannerLog(__func__, __LINE__, scanner, length)

#endif  // LL_USE == -1

static bool
prepLine(plLexicalScanner *scanner)
{
    while (scanner->line_length == 0) {
        unsigned int idx;

        if (!fgets(scanner->buffer, sizeof(scanner->buffer), scanner->file)) {
            if (ferror(scanner->file)) {
                SCANNER_ERROR("Failed to read from %s.", scanner->file_name);
                scanner->last_marker = PL_MARKER_READ_FAILURE;
            }
            else if (scanner->inside_comment_block) {
                SCANNER_ERROR("EOF encountered while inside comment block starting on line %u.",
                              scanner->comment_block_line_no);
                scanner->last_marker = PL_MARKER_BAD_DATA;
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

        if (scanner->inside_comment_block) {
            for (idx = 0; idx < scanner->line_length; idx++) {
                if (scanner->line[idx] == '*' && scanner->line[idx + 1] == '/') {
                    scanner->inside_comment_block = false;
                    idx += 2;
                    break;
                }
            }

            ADVANCE_SCANNER(scanner, idx);
        }

        if (scanner->line_length > 0) {
            VASQ_DEBUG(debug_logger, "%s, line %u: %.*s", scanner->file_name, scanner->location.line_no,
                       scanner->line_length, scanner->buffer);
        }
    }

    scanner->line[scanner->line_length] = '\0';

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

    ADVANCE_SCANNER(scanner, array->capacity + 1);  // The +1 is for the ending double quotes.

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

const char *
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

    if (position == 0) {
        const char *error_string;
        plLexicalLocation location;

        switch (level) {
        case VASQ_LL_ERROR: error_string = ERROR_STRING; break;

        case VASQ_LL_WARNING: error_string = WARNING_STRING; break;

        default: error_string = ""; break;
        }

        plGetLastLocation(scanner, &location);
        vasqIncSnprintf(dst, remaining, "%s%s:%u:%u", error_string, scanner->file_name, location.line_no,
                        location.column_no);
    }
    else {
        vasqIncSnprintf(dst, remaining, "%s", stripLineBeginning(scanner->buffer));
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

    options.flags = 0;
    options.processor = scannerProcessor;
    options.user = scanner;
    ret = vasqLoggerCreate(STDOUT_FILENO, VASQ_LL_WARNING, PL_LOGGER_PREAMBLE "%x: %M\n", &options,
                           &scanner->scanner_logger);
    if (ret != VASQ_RET_OK) {
        goto error;
    }

    options.processor = parserProcessor;
    ret = vasqLoggerCreate(STDOUT_FILENO, VASQ_LL_WARNING, PL_LOGGER_PREAMBLE "%x: %M\n\t%x\n", &options,
                           &scanner->parser_logger);
    if (ret != VASQ_RET_OK) {
        goto error;
    }

    scanner->file = file;
    scanner->file_name = file_name ? file_name : "<anonymous file>";
    scanner->last_marker = PL_MARKER_INIT;
    scanner->line = scanner->buffer;

    return PL_RET_OK;

error:

    plWordTableFree(scanner->table);
    vasqLoggerFree(scanner->scanner_logger);
    vasqLoggerFree(scanner->parser_logger);

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

read_token:

    if (!prepLine(scanner)) {
        goto return_marker;
    }

    token->data.handle = (plObjectHandle){0};
    token->header.submarker = PL_SUBMARKER_NONE;
    memcpy(&token->header.location, &scanner->location, sizeof(scanner->location));
    consumed = 1;

    switch (scanner->line[0]) {
    case ' ':
    case '\t':
        while (consumed < scanner->line_length && isWhitespace(scanner->line[consumed])) {
            consumed++;
        }
        ADVANCE_SCANNER(scanner, consumed);
        goto read_token;

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

    case '/':
        if (scanner->line[1] == '/') {
            scanner->line_length = 0;
            consumed = 0;
        }
        else if (scanner->line[1] == '*') {
            scanner->inside_comment_block = true;
            scanner->comment_block_line_no = scanner->location.line_no;

            for (consumed = 2; consumed < scanner->line_length; consumed++) {
                if (scanner->line[consumed] == '*' && scanner->line[consumed + 1] == '/') {
                    scanner->inside_comment_block = false;
                    consumed += 2;
                    break;
                }
            }
        }
        else {
            goto arithmetic_token;
        }

        ADVANCE_SCANNER(scanner, consumed);
        goto read_token;

    case '-':
        if (scanner->line[1] == '>') {
            scanner->last_marker = PL_MARKER_ARROW;
            consumed = 2;
            goto done;
        }
        /* FALLTHROUGH */
    case '+':
    case '*':
    case '%':
    case '|':
    case '&':
arithmetic_token:
        if (scanner->line[1] == '=') {
            scanner->last_marker = PL_MARKER_REASSIGNMENT;
            token->header.submarker = resolveArithmetic(scanner->line[0]);
            consumed = 2;
        }
        else {
            scanner->last_marker = PL_MARKER_ARITHMETIC;
            token->header.submarker = resolveArithmetic(scanner->line[0]);
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

            for (unsigned int k = 0; k < ARRAY_LENGTH(contexts); k++) {
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
    }

    for (unsigned int k = 0; k < ARRAY_LENGTH(keywords); k++) {
        if (strncmp(scanner->line, keywords[k].word, keywords[k].len) == 0 &&
            !isVarChar(scanner->line[keywords[k].len])) {
            scanner->last_marker = keywords[k].marker;

            if (scanner->last_marker == PL_MARKER_OBJECT) {
                token->data.handle = resolveStaticLiteral(scanner->line[0]);
            }
            else if (scanner->last_marker == PL_MARKER_LOGICAL) {
                token->header.submarker = (scanner->line[0] == 'o') ? PL_SUBMARKER_OR : PL_SUBMARKER_AND;
            }
            else if (scanner->last_marker == PL_MARKER_TYPE) {
                token->header.submarker = resolveType(scanner->line);
            }

            consumed = keywords[k].len;
            goto done;
        }
    }

    if (scanner->line[0] == '0' && scanner->line[1] == 'x') {
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
        ADVANCE_SCANNER(scanner, 1);
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
    ADVANCE_SCANNER(scanner, consumed);

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
