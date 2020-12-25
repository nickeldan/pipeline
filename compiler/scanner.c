#include <ctype.h>
#include <string.h>

#include "scanner.h"

struct keywordRecord {
    const char *word;
    unsigned int len;
    int marker;
};

static const struct keywordRecord keywords[] = {
    {"true", 4, PL_MARKER_OBJECT},   {"false", 4, PL_MARKER_OBJECT},  {"null", 4, PL_MARKER_OBJECT},
    {"blank", 5, PL_MARKER_OBJECT},  {"Any", 3, PL_MARKER_TYPE},      {"Num", 3, PL_MARKER_TYPE},
    {"Int", 3, PL_MARKER_TYPE},      {"Bool", 4, PL_MARKER_TYPE},     {"Float", 5, PL_MARKER_TYPE},
    {"Array", 5, PL_MARKER_TYPE},    {"GenArray", 8, PL_MARKER_TYPE}, {"Bytes", 5, PL_MARKER_TYPE},
    {"source", 6, PL_MARKER_SOURCE}, {"pipe", 4, PL_MARKER_PIPE},     {"sink", 4, PL_MARKER_SINK},
    {"local", 5, PL_MARKER_LOCAL},   {"struct", 6, PL_MARKER_STRUCT}, {"while", 5, PL_MARKER_WHILE},
    {"if", 2, PL_MARKER_IF},         {"eif", 3, PL_MARKER_EIF},       {"else", 4, PL_MARKER_ELSE},
    {"prod", 4, PL_MARKER_PROD},     {"drop", 4, PL_MARKER_DROP},     {"end", 3, PL_MARKER_END},
    {"not", 3, PL_MARKER_NOT},       {"or", 2, PL_MARKER_LOGICAL},    {"and", 3, PL_MARKER_LOGICAL},
    {"cont", 4, PL_MARKER_CONT},     {"break", 5, PL_MARKER_BREAK},   {"verify", 6, PL_MARKER_VERIFY},
    {"abort", 5, PL_MARKER_ABORT},   {"is", 2, PL_MARKER_IS},         {"as", 2, PL_MARKER_AS},
    {"import", 6, PL_MARKER_IMPORT}, {"export", 6, PL_MARKER_EXPORT}, {"main", 4, PL_MARKER_MAIN},
};

static const struct keywordRecord options[] = {
    {"STORE", 5, PL_SUBMARKER_STORE},
    {"ATTACH", 6, PL_SUBMARKER_ATTACH},
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

static plObject *
resolveStaticLiteral(char c)
{
    switch (c) {
    case 't': return &true_object;
    case 'f': return &false_object;
    case 'n': return &null_object;
    default: return &blank_object;
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
    scanner->line_length -= length;
}

#if LL_USE == VASQ_LL_RAWONLY

#define ADVANCE_SCANNER(scanner, length) advanceScanner(scanner, length)

#else

static void
advanceScannerLog(const char *function_name, unsigned int line_no, plLexicalScanner *scanner,
                  unsigned int length)
{
    advanceScanner(scanner, length);
    if (length > 0) {
        vasqLogStatement(VASQ_LL_DEBUG, __FILE__, function_name, line_no, "%u character%s consumed", length,
                         (length == 1) ? "" : "s");
    }
}
#define ADVANCE_SCANNER(scanner, length) advanceScannerLog(__func__, __LINE__, scanner, length)

#endif  // LL_USE == VASQ_LL_RAWONLY

static bool
prepLine(plLexicalScanner *scanner)
{
    while (scanner->line_length == 0) {
        unsigned int idx;

        if (!fgets(scanner->buffer, sizeof(scanner->buffer), scanner->file)) {
            if (ferror(scanner->file)) {
                VASQ_ERROR("Failed to read from %s", scanner->file_name);
                scanner->last_marker = PL_MARKER_READ_FAILURE;
            }
            else if (scanner->inside_comment_block) {
                COMPILER_ERROR("EOF encountered while inside comment block starting on line %u",
                               scanner->comment_block_line_no);
                scanner->last_marker = PL_MARKER_BAD_DATA;
            }
            else {
                scanner->last_marker = PL_MARKER_EOF;
            }
            return false;
        }

        scanner->line_no++;
        scanner->line_length = strnlen(scanner->buffer, sizeof(scanner->buffer));
        if (scanner->line_length >= sizeof(scanner->buffer)) {
            VASQ_RAWLOG("%s:%u: Line too long\n", scanner->file_name, scanner->line_no);
            scanner->last_marker = PL_MARKER_BAD_DATA;
            return false;
        }

        while (scanner->line_length > 0) {
            char c;

            c = scanner->buffer[scanner->line_length - 1];
            if (c == '\r' || c == '\n' || isWhitespace(c)) {
                scanner->line_length--;
            }
            else {
                break;
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
    }

    scanner->line[scanner->line_length] = '\0';

    return true;
}

static int
readByteString(plLexicalScanner *scanner, plObject **object)
{
    plByteArray *array;

    array = (plByteArray *)plNewByteArray();
    if (!array) {
        return PL_MARKER_OUT_OF_MEMORY;
    }

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

    COMPILER_ERROR("Unterminated string literal");
    goto error;

good_string:

    array->bytes = VASQ_MALLOC(array->capacity);
    if (!array->bytes) {
        plFreeObject((plObject *)array);
        return PL_MARKER_OUT_OF_MEMORY;
    }

    for (unsigned int k = 0; k < array->capacity; k++) {
        unsigned char c = scanner->line[k];

        if (!isprint(c) && c != '\t') {
            COMPILER_ERROR("Invalid byte in string literal: 0x%02x", c);
            goto error;
        }

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
                    COMPILER_ERROR("Unresolved hex byte in string literal");
                    goto error;
                }

                c = 0;
                for (int j = 0; j < 2; j++) {
                    char c2;
                    unsigned char x;

                    c2 = scanner->line[k + 1 + j];
                    if (!isxdigit(c2)) {
                        COMPILER_ERROR("Invalid hex byte in string literal");
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
                COMPILER_ERROR("Invalid escaped character in string literal: %c", scanner->line[k]);
                goto error;
            }
        }

        array->bytes[array->length++] = c;
    }

    ADVANCE_SCANNER(scanner, array->capacity + 1);  // The +1 is for the ending double quotes.

    *object = (plObject *)array;
    return PL_MARKER_OBJECT;

error:

    plFreeObject((plObject *)array);
    return PL_MARKER_BAD_DATA;
}

static void
lookaheadStoreLogic(plLexicalScanner *scanner, const plLexicalToken *token)
{
    if (scanner->num_look_ahead > 0) {
        unsigned int num_to_move;

        num_to_move = scanner->num_look_ahead;
        if (num_to_move == ARRAY_LENGTH(scanner->look_ahead)) {
            num_to_move--;
        }
        memmove(scanner->look_ahead + 1, scanner->look_ahead + 0, sizeof(*token) * num_to_move);
    }
    memcpy(scanner->look_ahead + 0, token, sizeof(*token));
    scanner->num_look_ahead++;
}

void
plScannerInit(plLexicalScanner *scanner, FILE *file, const char *file_name, plNameTable *table)
{
    if (!scanner || !file || !table) {
        VASQ_ERROR("scanner, file, and table cannot be NULL");
        return;
    }

    *scanner = (plLexicalScanner){0};

    scanner->file = file;
    scanner->file_name = file_name ? file_name : "<anonymous file>";
    scanner->table = table;
    scanner->last_marker = PL_MARKER_INIT;
    scanner->line = scanner->buffer;
}

int
plTokenRead(plLexicalScanner *scanner, plLexicalToken *token)
{
    unsigned int consumed;

    if (!scanner || !token) {
        VASQ_ERROR("The arguments cannot be NULL");
        return PL_MARKER_BAD_ARGS;
    }

    if (TERMINAL_MARKER(scanner->last_marker)) {
        goto return_marker;
    }

    if (scanner->num_look_ahead > 0) {
        memcpy(token, scanner->look_ahead + 0, sizeof(*token));
        scanner->last_marker = scanner->look_ahead[0].marker;
        scanner->last_look_ahead_line_no = scanner->look_ahead[0].line_no;
        if (scanner->num_look_ahead > 1) {
            memmove(scanner->look_ahead + 0, scanner->look_ahead + 1,
                    sizeof(*token) * (scanner->num_look_ahead - 1));
        }
        scanner->num_look_ahead--;
        goto return_marker;
    }
    else {
        scanner->last_look_ahead_line_no = 0;
    }

read_token:

    if (!prepLine(scanner)) {
        goto return_marker;
    }

    token->line_no = scanner->line_no;
    token->ctx.object = NULL;
    consumed = 1;

    switch (scanner->line[0]) {
    case ' ':
    case '\t':
        for (; consumed < scanner->line_length && isWhitespace(scanner->line[consumed]); consumed++) {}
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
            scanner->comment_block_line_no = scanner->line_no;

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
            token->ctx.submarker = resolveArithmetic(scanner->line[0]);
            consumed = 2;
        }
        else {
            scanner->last_marker = PL_MARKER_ARITHMETIC;
            token->ctx.submarker = resolveArithmetic(scanner->line[0]);
        }

        goto done;

    case '=':
    case '!':
        if (scanner->line[1] == '=') {
            scanner->last_marker = PL_MARKER_COMPARISON;
            token->ctx.submarker = (scanner->line[0] == '=') ? PL_SUBMARKER_EQUAL : PL_SUBMARKER_NOT_EQUAL;
            consumed = 2;
            goto done;
        }
        else {
            COMPILER_ERROR("Invalid token: '%c'", scanner->line[0]);
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

            token->ctx.submarker = (scanner->line[0] == '<') ? PL_SUBMARKER_LSHIFT : PL_SUBMARKER_RSHIFT;
        }
        else {
            if (scanner->line[1] == '=') {
                token->ctx.submarker =
                    (scanner->line[0] == '<') ? PL_SUBMARKER_LESS_THAN_EQ : PL_SUBMARKER_GREATER_THAN_EQ;
                consumed = 2;
            }
            else {
                token->ctx.submarker =
                    (scanner->line[0] == '<') ? PL_SUBMARKER_LESS_THAN : PL_SUBMARKER_GREATER_THAN;
            }

            scanner->last_marker = PL_MARKER_COMPARISON;
        }
        goto done;

    case '?':
        if (isVarChar(scanner->line[1])) {
            unsigned int end;

            for (unsigned int k = 0; k < ARRAY_LENGTH(options); k++) {
                if (strncmp(scanner->line + 1, options[k].word, options[k].len) == 0) {
                    scanner->last_marker = PL_MARKER_OPTION;
                    token->ctx.submarker = options[k].marker;
                    consumed = 1 + options[k].len;
                    goto done;
                }
            }

            for (end = 2; isVarChar(scanner->line[end]); end++) {}
            COMPILER_ERROR("Invalid option: %.*s", end - 1, scanner->line + 1);
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
                token->ctx.object = resolveStaticLiteral(scanner->line[0]);
            }
            else if (scanner->last_marker == PL_MARKER_LOGICAL) {
                token->ctx.submarker = (scanner->line[0] == 'o') ? PL_SUBMARKER_OR : PL_SUBMARKER_AND;
            }
            else if (scanner->last_marker == PL_MARKER_TYPE) {
                token->ctx.submarker = resolveType(scanner->line);
            }

            consumed = keywords[k].len;
            goto done;
        }
    }

    if (scanner->line[0] == '0' && scanner->line[1] == 'x') {
        for (consumed = 0; consumed < scanner->line_length; consumed++) {
            if (!isxdigit(scanner->line[consumed])) {
                if (isVarChar(scanner->line[consumed])) {
                    COMPILER_ERROR("Invalid hex literal");
                    scanner->last_marker = PL_MARKER_BAD_DATA;
                    goto return_marker;
                }

                break;
            }
        }

        if (plIntegerFromHexString(scanner->line + 2, consumed, &token->ctx.object) != PL_RET_OK) {
            scanner->last_marker = PL_MARKER_OUT_OF_MEMORY;
            goto return_marker;
        }
        consumed += 2;
    }
    else if (isdigit(scanner->line[0])) {
        int value;

        for (consumed = 1; consumed < scanner->line_length && isdigit(scanner->line[consumed]); consumed++) {
        }

        if (isVarChar(scanner->line[consumed])) {
            COMPILER_ERROR("Invalid numeric literal");
            scanner->last_marker = PL_MARKER_BAD_DATA;
            goto return_marker;
        }

        if (scanner->line[consumed] == '.') {
            for (consumed++; consumed < scanner->line_length && isdigit(scanner->line[consumed]);
                 consumed++) {}

            if (isVarChar(scanner->line[consumed])) {
                COMPILER_ERROR("Invalid numeric literal");
                scanner->last_marker = PL_MARKER_BAD_DATA;
                goto return_marker;
            }

            value = plFloatFromString(scanner->line, consumed, &token->ctx.object);
        }
        else {
            value = plIntegerFromString(scanner->line, consumed, &token->ctx.object);
        }

        if (value != PL_RET_OK) {
            if (value == PL_RET_OUT_OF_MEMORY) {
                scanner->last_marker = PL_MARKER_OUT_OF_MEMORY;
            }
            else {
                COMPILER_ERROR("Invalid numeric literal");
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
            token->ctx.name = plRegisterName(scanner->table, scanner->line, consumed, NULL);
            if (!token->ctx.name) {
                scanner->last_marker = PL_MARKER_OUT_OF_MEMORY;
                goto return_marker;
            }

            scanner->last_marker = PL_MARKER_NAME;
        }
    }
    else if (scanner->line[0] == '"') {
        ADVANCE_SCANNER(scanner, 1);
        scanner->last_marker = readByteString(scanner, &token->ctx.object);
        if (TERMINAL_MARKER(scanner->last_marker)) {
            goto return_marker;
        }
        consumed = 0;  // readByteString already advanced the scanner.
    }
    else {
        if (isprint(scanner->line[0])) {
            COMPILER_ERROR("Unexpected character: %c", scanner->line[0]);
        }
        else {
            COMPILER_ERROR("Unprintable character: 0x%02x", scanner->line[0]);
        }

        scanner->last_marker = PL_MARKER_BAD_DATA;
        goto return_marker;
    }

done:

    token->marker = scanner->last_marker;
    ADVANCE_SCANNER(scanner, consumed);

return_marker:

    return scanner->last_marker;
}

unsigned int
plLastLineNo(const plLexicalScanner *scanner)
{
    if (!scanner) {
        VASQ_ERROR("scanner cannot be NULL");
        return 0;
    }

    if (scanner->last_look_ahead_line_no > 0) {
        return scanner->last_look_ahead_line_no;
    }
    else {
        return scanner->line_no;
    }
}

void
plTokenCleanup(plLexicalToken *token, plNameTable *table)
{
    if (!token) {
        VASQ_ERROR("token cannot be NULL");
        return;
    }

    switch (token->marker) {
    case PL_MARKER_NAME: plUnregisterName(table, token->ctx.name); break;

    case PL_MARKER_OBJECT: plFreeObject(token->ctx.object); break;

    default: break;
    }
}

const char *
plLexicalMarkerName(int marker)
{
    switch (marker) {
    case PL_MARKER_ARROW: return "ARROW";
    case PL_MARKER_NAME: return "NAME";
    case PL_MARKER_OBJECT: return "OBJECT";
    case PL_MARKER_TYPE: return "TYPE";
    case PL_MARKER_SOURCE: return "SOURCE";
    case PL_MARKER_PIPE: return "PIPE";
    case PL_MARKER_SINK: return "SINK";
    case PL_MARKER_LOCAL: return "LOCAL";
    case PL_MARKER_STRUCT: return "STRUCT";
    case PL_MARKER_WHILE: return "WHILE";
    case PL_MARKER_IF: return "IF";
    case PL_MARKER_EIF: return "EIF";
    case PL_MARKER_ELSE: return "ELSE";
    case PL_MARKER_PROD: return "PROD";
    case PL_MARKER_DROP: return "DROP";
    case PL_MARKER_END: return "END";
    case PL_MARKER_NOT: return "NOT";
    case PL_MARKER_LOGICAL: return "LOGICAL";
    case PL_MARKER_CONT: return "CONT";
    case PL_MARKER_BREAK: return "BREAK";
    case PL_MARKER_VERIFY: return "VERIFY";
    case PL_MARKER_ABORT: return "ABORT";
    case PL_MARKER_IS: return "IS";
    case PL_MARKER_AS: return "AS";
    case PL_MARKER_IMPORT: return "IMPORT";
    case PL_MARKER_EXPORT: return "EXPORT";
    case PL_MARKER_MAIN: return "MAIN";
    case PL_MARKER_SEMICOLON: return "';'";
    case PL_MARKER_COLON: return "':'";
    case PL_MARKER_COMMA: return "','";
    case PL_MARKER_PERIOD: return "'.'";
    case PL_MARKER_QUESTION: return "'?'";
    case PL_MARKER_UNDERSCORE: return "'_'";
    case PL_MARKER_LEFT_PARENS: return "'('";
    case PL_MARKER_RIGHT_PARENS: return "')'";
    case PL_MARKER_LEFT_BRACKET: return "'['";
    case PL_MARKER_RIGHT_BRACKET: return "']'";
    case PL_MARKER_LEFT_BRACE: return "'{'";
    case PL_MARKER_RIGHT_BRACE: return "'}'";
    case PL_MARKER_ARITHMETIC: return "ARITHMETIC";
    case PL_MARKER_REASSIGNMENT: return "REASSIGNMENT";
    case PL_MARKER_COMPARISON: return "COMPARISON";
    case PL_MARKER_OPTION: return "OPTION";
    default: return "";
    }
}

const char *
plStripLineBeginning(const char *line)
{
    const char *ret;

    for (ret = line; isWhitespace(*ret); ret++) {}

    return ret;
}

#if LL_USE == VASQ_LL_RAWONLY

int
plLookaheadStoreNoLog(plLexicalScanner *scanner, const plLexicalToken *token)
{
    if (!scanner || !token || scanner->num_look_ahead == ARRAY_LENGTH(scanner->look_ahead)) {
        return PL_RET_USAGE;
    }

    lookaheadStoreLogic(scanner, token);
    return PL_RET_OK;
}

#else  // LL_USE == VASQ_LL_RAWONLY

int
plTokenReadLog(const char *file_name, const char *function_name, unsigned int line_no,
               plLexicalScanner *scanner, plLexicalToken *token)
{
    int ret;

    ret = plTokenRead(scanner, token);
    if (!TERMINAL_MARKER(ret)) {
        vasqLogStatement(VASQ_LL_INFO, file_name, function_name, line_no, "Read token: %s",
                         plLexicalMarkerName(ret));
    }
    else if (ret == PL_MARKER_EOF) {
        vasqLogStatement(VASQ_LL_INFO, file_name, function_name, line_no, "End of file reached");
    }
    return ret;
}

int
plLookaheadStoreLog(const char *file_name, const char *function_name, unsigned int line_no,
                    plLexicalScanner *scanner, const plLexicalToken *token)
{
    if (!scanner || !token) {
        VASQ_ERROR("The arguments cannot be NULL");
        return PL_RET_USAGE;
    }

    if (scanner->num_look_ahead == ARRAY_LENGTH(scanner->look_ahead)) {
        vasqLogStatement(VASQ_LL_ERROR, file_name, function_name, line_no,
                         "Cannot store any more look ahead tokens");
        return PL_RET_USAGE;
    }

    lookaheadStoreLogic(scanner, token);
    vasqLogStatement(VASQ_LL_DEBUG, file_name, function_name, line_no, "%s stored as look ahead",
                     plLexicalMarkerName(token->marker));
    return PL_RET_OK;
}

#endif  // LL_USE == VASQ_LL_RAWONLY
