#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "scanner.h"
#include "util.h"

struct keywordRecord {
    const char *word;
    unsigned int len;
    plLexicalMarker_t marker;
};

struct optionRecord {
    const char *word;
    unsigned int len;
    plLexicalSubmarker_t submarker;
};

#define SCANNER_ERROR(format, ...) do { \
    VASQ_RAWLOG("%s:%u: " format "\n", scanner->file_name, scanner->line_no, ##__VA_ARGS__); \
    VASQ_RAWLOG("\t%s\n", stripLineBeginning(scanner->buffer)); \
} while(0)

static const struct keywordRecord keywords[] = {
    {"true", 4, PL_LMARKER_LITERAL},  {"false", 4, PL_LMARKER_LITERAL}, {"null", 4, PL_LMARKER_LITERAL},
    {"blank", 5, PL_LMARKER_LITERAL}, {"Num", 3, PL_LMARKER_TYPE},      {"Int", 3, PL_LMARKER_TYPE},
    {"Bool", 4, PL_LMARKER_TYPE},     {"Float", 5, PL_LMARKER_TYPE},    {"Array", 5, PL_LMARKER_TYPE},
    {"GenArray", 8, PL_LMARKER_TYPE}, {"Bytes", 5, PL_LMARKER_TYPE},    {"source", 6, PL_LMARKER_SOURCE},
    {"pipe", 4, PL_LMARKER_PIPE},     {"sink", 4, PL_LMARKER_SINK},     {"local", 5, PL_LMARKER_LOCAL},
    {"struct", 6, PL_LMARKER_STRUCT}, {"while", 5, PL_LMARKER_WHILE},   {"if", 2, PL_LMARKER_IF},
    {"eif", 3, PL_LMARKER_EIF},       {"else", 4, PL_LMARKER_ELSE},     {"prod", 4, PL_LMARKER_PROD},
    {"drop", 4, PL_LMARKER_DROP},     {"end", 3, PL_LMARKER_END},       {"not", 3, PL_LMARKER_NOT},
    {"or", 2, PL_LMARKER_LOGICAL},    {"and", 3, PL_LMARKER_LOGICAL},   {"cont", 4, PL_LMARKER_CONT},
    {"break", 5, PL_LMARKER_BREAK},   {"verify", 6, PL_LMARKER_VERIFY}, {"abort", 5, PL_LMARKER_ABORT},
    {"is", 2, PL_LMARKER_IS},         {"as", 2, PL_LMARKER_AS},         {"import", 6, PL_LMARKER_IMPORT},
    {"export", 6, PL_LMARKER_EXPORT}, {"main", 4, PL_LMARKER_MAIN},
};

static const struct optionRecord options[] = {
    {"STORE", 5, PL_LSUBMARKER_STORE},
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

static const char *
stripLineBeginning(const char *line)
{
    const char *ret;

    for (ret=line; isWhitespace(*ret); ret++) {}

    return ret;
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

static plLexicalSubmarker_t
resolveType(const char *word)
{
    switch (word[0]) {
    case 'I': return PL_LSUBMARKER_INT;
    case 'F': return PL_LSUBMARKER_FLOAT;
    case 'N': return PL_LSUBMARKER_NUM;
    case 'A': return PL_LSUBMARKER_ARRAY;
    case 'G': return PL_LSUBMARKER_GENARRAY;
    default:  // 'B'
        return (word[1] == 'o') ? PL_LSUBMARKER_BOOL : PL_LSUBMARKER_BYTES;
    }
}

static plLexicalSubmarker_t
resolveArithmetic(char c)
{
    switch (c) {
    case '+': return PL_LSUBMARKER_PLUS;
    case '-': return PL_LSUBMARKER_MINUS;
    case '*': return PL_LSUBMARKER_MULTIPLY;
    case '/': return PL_LSUBMARKER_DIVIDE;
    case '%': return PL_LSUBMARKER_MODULO;
    case '|': return PL_LSUBMARKER_OR;
    default: return PL_LSUBMARKER_AND;  // &
    }
}

static void
advanceScanner(plLexicalScanner *scanner, unsigned int length)
{
    scanner->line += length;
    scanner->line_length -= length;
}

static bool
prepLine(plLexicalScanner *scanner)
{
    while (scanner->line_length == 0) {
        unsigned int idx;

        if (!fgets(scanner->buffer, sizeof(scanner->buffer), scanner->file)) {
            if (ferror(scanner->file)) {
                VASQ_ERROR("Failed to read from %s", scanner->file_name);
                scanner->last_marker = PL_LMARKER_READ_FAILURE;
            }
            else if (scanner->inside_comment_block) {
                SCANNER_ERROR("EOF encountered while inside comment block starting on line %u",
                              scanner->comment_block_line_no);
                scanner->last_marker = PL_LMARKER_BAD_DATA;
            }
            else {
                scanner->last_marker = PL_LMARKER_EOF;
            }
            return false;
        }

        scanner->line_no++;
        scanner->line_length = strnlen(scanner->buffer, sizeof(scanner->buffer));
        if (scanner->line_length >= sizeof(scanner->buffer)) {
            SCANNER_ERROR("Line too long");
            scanner->last_marker = PL_LMARKER_BAD_DATA;
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

            advanceScanner(scanner, idx);
        }
    }

    scanner->line[scanner->line_length] = '\0';

    return true;
}

static plLexicalMarker_t
readByteString(plLexicalScanner *scanner, plObject **object)
{
    plByteArray *array;

    array = (plByteArray *)plNewByteArray();
    if (!array) {
        return PL_LMARKER_OUT_OF_MEMORY;
    }

    for (unsigned int k=0; k<scanner->line_length; k++) {
        char c;

        c = scanner->line[k];
        if ( c == '\\' && ++k == scanner->line_length ) {
            break;
        }

        if ( c == '"' ) {
            array->capacity = k;
            goto good_string;
        }
    }

    SCANNER_ERROR("Unterminated string literal");
    goto error;

good_string:

    array->bytes = VASQ_MALLOC(array->capacity);
    if (!array->bytes) {
        plFreeObject((plObject *)array);
        return PL_LMARKER_OUT_OF_MEMORY;
    }

    for (unsigned int k = 0; k < array->capacity; k++) {
        unsigned char c;

        c = scanner->line[k];

        if (c == '\\') {
            if (k + 1 == scanner->line_length) {
                SCANNER_ERROR("Unresolved escape character in string literal");
                goto error;
            }

            switch (scanner->line[++k]) {
            case 't': c = '\t'; break;
            case 'n': c = '\n'; break;
            case 'r': c = '\r'; break;
            case '\\': c = '\\'; break;
            case '"': c = '"'; break;
            case 'x':
                if (k + 2 >= scanner->line_length) {
                    SCANNER_ERROR("Unresolved hex byte in string literal");
                    goto error;
                }

                c = 0;
                for (int j = 0; j < 2; j++) {
                    char c2;
                    unsigned char x;

                    c2 = scanner->line[k + 1 + j];
                    if (!isxdigit(c2)) {
                        SCANNER_ERROR("Invalid hex byte in string literal");
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
                SCANNER_ERROR("Invalied escaped character in string literal: %c", scanner->line[k]);
                goto error;
            }
        }
        else if (isprint(c) || c == '\t') {
        }
        else {
            SCANNER_ERROR("Invalid byte in string literal: 0x%02x", c);
            goto error;
        }

        array->bytes[array->length++] = c;
    }

    advanceScanner(scanner, array->capacity+1); // The +1 is for the ending double quotes.

    *object = (plObject *)array;
    return PL_LMARKER_LITERAL;

error:

    plFreeObject((plObject *)array);
    return PL_LMARKER_BAD_DATA;
}

void
plScannerInit(plLexicalScanner *scanner, FILE *file, const char *file_name)
{
    if (!scanner || !file) {
        VASQ_ERROR("scanner and file cannot be NULL");
        return;
    }

    scanner->file = file;
    scanner->file_name = file_name ? file_name : "<anonymous file>";
    scanner->line_no = 0;
    scanner->line_length = 0;
    scanner->inside_comment_block = false;
    scanner->last_marker = PL_LMARKER_INIT;
    scanner->buffer[0] = '\0';
    scanner->line = scanner->buffer;
}

plLexicalMarker_t
plTokenRead(plLexicalScanner *scanner, plLexicalToken *token, plNameTable *table)
{
    unsigned int consumed;

    if (!scanner || !token) {
        VASQ_ERROR("The arguments cannot be NULL");
        return PL_LMARKER_BAD_ARGS;
    }

    *token = (plLexicalToken){0};

    if (TERMINAL_LMARKER(scanner->last_marker) ) {
        goto return_marker;
    }

read_token:

    if ( !prepLine(scanner) ) {
        goto return_marker;
    }

    token->line_no = scanner->line_no;
    token->ctx.object = NULL;
    consumed = 1;

    switch (scanner->line[0]) {
    case ' ':
    case '\t':
        for (; consumed < scanner->line_length && isWhitespace(scanner->line[consumed]); consumed++) {}
        advanceScanner(scanner, consumed);
        goto read_token;

    case ';': scanner->last_marker = PL_LMARKER_SEMICOLON; goto done;

    case ':': scanner->last_marker = PL_LMARKER_COLON; goto done;

    case ',': scanner->last_marker = PL_LMARKER_COMMA; goto done;

    case '.': scanner->last_marker = PL_LMARKER_PERIOD; goto done;

    case '(': scanner->last_marker = PL_LMARKER_LEFT_PARENS; goto done;

    case ')': scanner->last_marker = PL_LMARKER_RIGHT_PARENS; goto done;

    case '{': scanner->last_marker = PL_LMARKER_LEFT_BRACE; goto done;

    case '}': scanner->last_marker = PL_LMARKER_RIGHT_BRACE; goto done;

    case '[': scanner->last_marker = PL_LMARKER_LEFT_BRACKET; goto done;

    case ']': scanner->last_marker = PL_LMARKER_RIGHT_BRACKET; goto done;

    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '|':
    case '&':
        if (scanner->line[1] == '=') {
            scanner->last_marker = PL_LMARKER_REASSIGNMENT;
            token->submarker = resolveArithmetic(scanner->line[0]);
            consumed = 2;
        }
        else if (scanner->line[0] == '-' && scanner->line[1] == '>') {
            scanner->last_marker = PL_LMARKER_ARROW;
            consumed = 2;
        }
        else if (scanner->line[0] == '/') {
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
                goto done;
            }

            advanceScanner(scanner, consumed);
            goto read_token;
        }
        else {
            scanner->last_marker = PL_LMARKER_ARITHMETIC;
            token->submarker = resolveArithmetic(scanner->line[0]);
        }

        goto done;

    case '=':
    case '!':
        if ( scanner->line[1] == '=' ) {
            scanner->last_marker = PL_LMARKER_COMPARISON;
            token->submarker = ( scanner->line[0] == '=' )? PL_LSUBMARKER_EQUAL : PL_LSUBMARKER_NOT_EQUAL;
            consumed = 2;
            goto done;
        }
        else {
            SCANNER_ERROR("Invalid token: '%c'", scanner->line[0]);
            scanner->last_marker = PL_LMARKER_BAD_DATA;
            goto return_marker;
        }

    case '<':
    case '>':
        if (scanner->line[1] == scanner->line[0]) {
            if (scanner->line[2] == '=') {
                scanner->last_marker = PL_LMARKER_REASSIGNMENT;
                consumed = 3;
            }
            else {
                scanner->last_marker = PL_LMARKER_ARITHMETIC;
                consumed = 2;
            }

            token->submarker = (scanner->line[0] == '<') ? PL_LSUBMARKER_LSHIFT : PL_LSUBMARKER_RSHIFT;
        }
        else {
            if (scanner->line[1] == '=') {
                token->submarker =
                    (scanner->line[0] == '<') ? PL_LSUBMARKER_LESS_THAN_EQ : PL_LSUBMARKER_GREATER_THAN_EQ;
                consumed = 2;
            }
            else {
                token->submarker =
                    (scanner->line[0] == '<') ? PL_LSUBMARKER_LESS_THAN : PL_LSUBMARKER_GREATER_THAN;
            }

            scanner->last_marker = PL_LMARKER_COMPARISON;
        }
        goto done;

    case '?':
        if (isVarChar(scanner->line[1])) {
            unsigned int end;

            for (unsigned int k = 0; k < ARRAY_LENGTH(options); k++) {
                if (strncmp(scanner->line + 1, options[k].word, options[k].len) == 0) {
                    scanner->last_marker = PL_LMARKER_OPTION;
                    token->submarker = options[k].submarker;
                    consumed = 1 + options[k].len;
                    goto done;
                }
            }

            for (end = 1; end < scanner->line_length && isVarChar(scanner->line[end]); end++) {}
            SCANNER_ERROR("Invalid option: %.*s", end - 1, scanner->line + 1);
            scanner->last_marker = PL_LMARKER_BAD_DATA;
            goto return_marker;
        }
        else {
            scanner->last_marker = PL_LMARKER_QUESTION;
            goto done;
        }
    }

    for (unsigned int k = 0; k < ARRAY_LENGTH(keywords); k++) {
        if (strncmp(scanner->line, keywords[k].word, keywords[k].len) == 0 &&
            !isVarChar(scanner->line[keywords[k].len])) {
            scanner->last_marker = keywords[k].marker;

            if (scanner->last_marker == PL_LMARKER_LITERAL) {
                token->ctx.object = resolveStaticLiteral(scanner->line[0]);
            }
            else if (scanner->last_marker == PL_LMARKER_LOGICAL) {
                token->submarker = (scanner->line[0] == 'o') ? PL_LSUBMARKER_OR : PL_LSUBMARKER_AND;
            }
            else if (scanner->last_marker == PL_LMARKER_TYPE) {
                token->submarker = resolveType(scanner->line);
            }

            consumed = keywords[k].len;
            goto done;
        }
    }

    if (scanner->line[0] == '0' && scanner->line[1] == 'x') {
        for (consumed = 0; consumed < scanner->line_length; consumed++) {
            if (!isxdigit(scanner->line[consumed])) {
                if (isVarChar(scanner->line[consumed])) {
                    SCANNER_ERROR("Invalid hex literal");
                    scanner->last_marker = PL_LMARKER_BAD_DATA;
                    goto return_marker;
                }

                break;
            }
        }

        if (plIntegerFromHexString(scanner->line + 2, consumed, &token->ctx.object) != PL_RET_OK) {
            scanner->last_marker = PL_LMARKER_OUT_OF_MEMORY;
            goto return_marker;
        }
        consumed += 2;
    }
    else if (isdigit(scanner->line[0])) {
        int value;

        for (consumed = 1; consumed < scanner->line_length && isdigit(scanner->line[consumed]); consumed++) {
        }

        if (isVarChar(scanner->line[consumed])) {
            SCANNER_ERROR("Invalid numeric literal");
            scanner->last_marker = PL_LMARKER_BAD_DATA;
            goto return_marker;
        }

        if (scanner->line[consumed] == '.') {
            for (consumed++; consumed < scanner->line_length && isdigit(scanner->line[consumed]);
                 consumed++) {}

            if (isVarChar(scanner->line[consumed])) {
                SCANNER_ERROR("Invalid numeric literal");
                scanner->last_marker = PL_LMARKER_BAD_DATA;
                goto return_marker;
            }

            value = plFloatFromString(scanner->line, consumed, &token->ctx.object);
        }
        else {
            value = plIntegerFromString(scanner->line, consumed, &token->ctx.object);
        }

        if (value != PL_RET_OK) {
            if (value == PL_RET_OUT_OF_MEMORY) {
                scanner->last_marker = PL_LMARKER_OUT_OF_MEMORY;
            }
            else {
                SCANNER_ERROR("Invalid numeric literal");
                scanner->last_marker = PL_LMARKER_BAD_DATA;
            }
                goto return_marker;
        }

        scanner->last_marker = PL_LMARKER_LITERAL;
    }
    else if (isVarChar(scanner->line[0])) {
        for (; consumed < scanner->line_length && isVarChar(scanner->line[consumed]); consumed++) {}

        if (consumed == 1 && scanner->line[0] == '_') {
            scanner->last_marker = PL_LMARKER_UNDERSCORE;
        }
        else {
            token->ctx.name = plRegisterName(table, scanner->line, consumed, NULL);
            if (!token->ctx.name) {
                scanner->last_marker = PL_LMARKER_OUT_OF_MEMORY;
                goto return_marker;
            }

            scanner->last_marker = PL_LMARKER_NAME;
        }
    }
    else if (scanner->line[0] == '"') {
        advanceScanner(scanner, 1);
        scanner->last_marker = readByteString(scanner, &token->ctx.object);
        if (TERMINAL_LMARKER(scanner->last_marker)) {
            goto return_marker;
        }
        consumed = 0; // readByteString already advanced the scanner.
    }
    else {
        if (isprint(scanner->line[0])) {
            SCANNER_ERROR("Unexpected character: %c", scanner->line[0]);
        }
        else {
            SCANNER_ERROR("Unprintable character: 0x%02x", scanner->line[0]);
        }

        scanner->last_marker = PL_LMARKER_BAD_DATA;
        goto return_marker;
    }

done:

    token->marker = scanner->last_marker;
    advanceScanner(scanner, consumed);

return_marker:

    return scanner->last_marker;
}

void
plTokenCleanup(plLexicalToken *token, plNameTable *table)
{
    if ( !token || !table ) {
        VASQ_ERROR("The arguments cannot be NULL");
        return;
    }

    switch ( token->marker ) {
    case PL_LMARKER_NAME:
        plUnregisterName(table, token->ctx.name);
        break;

    case PL_LMARKER_LITERAL:
        plFreeObject(token->ctx.object);
        break;
    }
}

const char *
plLexicalMarkerName(plLexicalMarker_t marker)
{
    switch ( marker ) {
    case PL_LMARKER_ARROW: return "ARROW";
    case PL_LMARKER_NAME: return "NAME";
    case PL_LMARKER_LITERAL: return "LITERAL";
    case PL_LMARKER_TYPE: return "TYPE";
    case PL_LMARKER_SOURCE: return "SOURCE";
    case PL_LMARKER_PIPE: return "PIPE";
    case PL_LMARKER_SINK: return "SINK";
    case PL_LMARKER_LOCAL: return "LOCAL";
    case PL_LMARKER_STRUCT: return "STRUCT";
    case PL_LMARKER_WHILE: return "WHILE";
    case PL_LMARKER_IF: return "IF";
    case PL_LMARKER_EIF: return "EIF";
    case PL_LMARKER_ELSE: return "ELSE";
    case PL_LMARKER_PROD: return "PROD";
    case PL_LMARKER_DROP: return "DROP";
    case PL_LMARKER_END: return "END";
    case PL_LMARKER_NOT: return "NOT";
    case PL_LMARKER_LOGICAL: return "LOGICAL";
    case PL_LMARKER_CONT: return "CONT";
    case PL_LMARKER_BREAK: return "BREAK";
    case PL_LMARKER_VERIFY: return "VERIFY";
    case PL_LMARKER_ABORT: return "ABORT";
    case PL_LMARKER_IS: return "IS";
    case PL_LMARKER_AS: return "AS";
    case PL_LMARKER_IMPORT: return "IMPORT";
    case PL_LMARKER_EXPORT: return "EXPORT";
    case PL_LMARKER_MAIN: return "MAIN";
    case PL_LMARKER_SEMICOLON: return ";";
    case PL_LMARKER_COLON: return ":";
    case PL_LMARKER_COMMA: return ",";
    case PL_LMARKER_PERIOD: return ".";
    case PL_LMARKER_QUESTION: return "?";
    case PL_LMARKER_UNDERSCORE: return "_";
    case PL_LMARKER_LEFT_PARENS: return "(";
    case PL_LMARKER_RIGHT_PARENS: return ")";
    case PL_LMARKER_LEFT_BRACKET: return "[";
    case PL_LMARKER_RIGHT_BRACKET: return "]";
    case PL_LMARKER_LEFT_BRACE: return "{";
    case PL_LMARKER_RIGHT_BRACE: return "}";
    case PL_LMARKER_ARITHMETIC: return "ARITHMETIC";
    case PL_LMARKER_REASSIGNMENT: return "REASSIGNMENT";
    case PL_LMARKER_COMPARISON: return "COMPARISON";
    case PL_LMARKER_OPTION: return "OPTION";
    default: return "";
    }
}
