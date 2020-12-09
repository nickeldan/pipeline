#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "nameTable.h"
#include "plObject.h"

enum plLexicalMarker {
    PL_LMARKER_BAD_ARGS = 256,
    PL_LMARKER_READ_FAILURE,
    PL_LMARKER_BAD_DATA,
    PL_LMARKER_OUT_OF_MEMORY,
    PL_LMARKER_EOF,

    PL_LMARKER_INIT,
    PL_LMARKER_ARROW,
    PL_LMARKER_NAME,
    PL_LMARKER_LITERAL,
    PL_LMARKER_TYPE,
    PL_LMARKER_SOURCE,
    PL_LMARKER_PIPE,
    PL_LMARKER_SINK,
    PL_LMARKER_LOCAL,
    PL_LMARKER_STRUCT,
    PL_LMARKER_WHILE,
    PL_LMARKER_IF,
    PL_LMARKER_EIF,
    PL_LMARKER_ELSE,
    PL_LMARKER_PROD,
    PL_LMARKER_DROP,
    PL_LMARKER_END,
    PL_LMARKER_NOT,
    PL_LMARKER_LOGICAL,
    PL_LMARKER_CONT,
    PL_LMARKER_BREAK,
    PL_LMARKER_VERIFY,
    PL_LMARKER_ABORT,
    PL_LMARKER_IS,
    PL_LMARKER_AS,
    PL_LMARKER_IMPORT,
    PL_LMARKER_EXPORT,
    PL_LMARKER_MAIN,
    PL_LMARKER_SEMICOLON,
    PL_LMARKER_COLON,
    PL_LMARKER_COMMA,
    PL_LMARKER_PERIOD,
    PL_LMARKER_QUESTION,
    PL_LMARKER_UNDERSCORE,
    PL_LMARKER_LEFT_PARENS,
    PL_LMARKER_RIGHT_PARENS,
    PL_LMARKER_LEFT_BRACKET,
    PL_LMARKER_RIGHT_BRACKET,
    PL_LMARKER_LEFT_BRACE,
    PL_LMARKER_RIGHT_BRACE,
    PL_LMARKER_ARITHMETIC,
    PL_LMARKER_REASSIGNMENT,
    PL_LMARKER_COMPARISON,
    PL_LMARKER_OPTION,
};
#define TERMINAL_LMARKER(marker) ((marker) <= PL_LMARKER_EOF)

enum plLexicalSubmarker {
    PL_LSUBMARKER_OR = 0,
    PL_LSUBMARKER_AND,
    PL_LSUBMARKER_INT,
    PL_LSUBMARKER_FLOAT,
    PL_LSUBMARKER_NUM,
    PL_LSUBMARKER_ARRAY,
    PL_LSUBMARKER_GENARRAY,
    PL_LSUBMARKER_BOOL,
    PL_LSUBMARKER_BYTES,
    PL_LSUBMARKER_PLUS,
    PL_LSUBMARKER_MINUS,
    PL_LSUBMARKER_MULTIPLY,
    PL_LSUBMARKER_DIVIDE,
    PL_LSUBMARKER_MODULO,
    PL_LSUBMARKER_LSHIFT,
    PL_LSUBMARKER_RSHIFT,
    PL_LSUBMARKER_EQUAL,
    PL_LSUBMARKER_NOT_EQUAL,
    PL_LSUBMARKER_LESS_THAN,
    PL_LSUBMARKER_LESS_THAN_EQ,
    PL_LSUBMARKER_GREATER_THAN,
    PL_LSUBMARKER_GREATER_THAN_EQ,
    PL_LSUBMARKER_STORE,
};

typedef struct plLexicalToken {
    union {
        const char *name;
        plObject *object;
        int submarker;
    } ctx;
    int marker;
    unsigned int line_no;
} plLexicalToken;

typedef struct plLexicalScanner {
    FILE *file;
    const char *file_name;
    plNameTable *table;
    char *line;
    unsigned int line_no;
    unsigned int comment_block_line_no;
    unsigned int line_length;
    int last_marker;
    bool inside_comment_block;
    char buffer[200];
} plLexicalScanner;

void
plScannerInit(plLexicalScanner *scanner, FILE *file, const char *file_name, plNameTable *table);

int
plTokenRead(plLexicalScanner *scanner, plLexicalToken *token);

void
plTokenCleanup(plLexicalToken *token, plNameTable *table);

const char *
plLexicalMarkerName(int marker);
