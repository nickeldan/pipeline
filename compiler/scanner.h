#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "definitions.h"
#include "nameTable.h"
#include "plObject.h"

enum plLexicalMarker {
    PL_MARKER_BAD_ARGS = 256,
    PL_MARKER_READ_FAILURE,
    PL_MARKER_BAD_DATA,
    PL_MARKER_OUT_OF_MEMORY,
    PL_MARKER_EOF,

    PL_MARKER_INIT,
    PL_MARKER_ARROW,
    PL_MARKER_NAME,
    PL_MARKER_LITERAL,
    PL_MARKER_TYPE,
    PL_MARKER_SOURCE,
    PL_MARKER_PIPE,
    PL_MARKER_SINK,
    PL_MARKER_LOCAL,
    PL_MARKER_STRUCT,
    PL_MARKER_WHILE,
    PL_MARKER_IF,
    PL_MARKER_EIF,
    PL_MARKER_ELSE,
    PL_MARKER_PROD,
    PL_MARKER_DROP,
    PL_MARKER_END,
    PL_MARKER_NOT,
    PL_MARKER_LOGICAL,
    PL_MARKER_CONT,
    PL_MARKER_BREAK,
    PL_MARKER_VERIFY,
    PL_MARKER_ABORT,
    PL_MARKER_IS,
    PL_MARKER_AS,
    PL_MARKER_IMPORT,
    PL_MARKER_EXPORT,
    PL_MARKER_MAIN,
    PL_MARKER_SEMICOLON,
    PL_MARKER_COLON,
    PL_MARKER_COMMA,
    PL_MARKER_PERIOD,
    PL_MARKER_QUESTION,
    PL_MARKER_UNDERSCORE,
    PL_MARKER_LEFT_PARENS,
    PL_MARKER_RIGHT_PARENS,
    PL_MARKER_LEFT_BRACKET,
    PL_MARKER_RIGHT_BRACKET,
    PL_MARKER_LEFT_BRACE,
    PL_MARKER_RIGHT_BRACE,
    PL_MARKER_ARITHMETIC,
    PL_MARKER_REASSIGNMENT,
    PL_MARKER_COMPARISON,
    PL_MARKER_OPTION,
};
#define TERMINAL_MARKER(marker) ((marker) <= PL_MARKER_EOF)

enum plLexicalSubmarker {
    PL_SUBMARKER_INIT = 0,

    PL_SUBMARKER_OR,
    PL_SUBMARKER_AND,
    PL_SUBMARKER_INT,
    PL_SUBMARKER_FLOAT,
    PL_SUBMARKER_NUM,
    PL_SUBMARKER_ARRAY,
    PL_SUBMARKER_GENARRAY,
    PL_SUBMARKER_BOOL,
    PL_SUBMARKER_BYTES,
    PL_SUBMARKER_PLUS,
    PL_SUBMARKER_MINUS,
    PL_SUBMARKER_MULTIPLY,
    PL_SUBMARKER_DIVIDE,
    PL_SUBMARKER_MODULO,
    PL_SUBMARKER_LSHIFT,
    PL_SUBMARKER_RSHIFT,
    PL_SUBMARKER_EQUAL,
    PL_SUBMARKER_NOT_EQUAL,
    PL_SUBMARKER_LESS_THAN,
    PL_SUBMARKER_LESS_THAN_EQ,
    PL_SUBMARKER_GREATER_THAN,
    PL_SUBMARKER_GREATER_THAN_EQ,
    PL_SUBMARKER_STORE,
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
    plLexicalToken look_ahead[2];
    FILE *file;
    const char *file_name;
    plNameTable *table;
    char *line;
    unsigned int line_no;
    unsigned int comment_block_line_no;
    unsigned int line_length;
    int last_marker;
    bool inside_comment_block;
    unsigned char num_look_ahead;
    char buffer[200];
} plLexicalScanner;

void
plScannerInit(plLexicalScanner *scanner, FILE *file, const char *file_name, plNameTable *table);

int
plTokenRead(plLexicalScanner *scanner, plLexicalToken *token);

#if LL_USE == VASQ_LL_RAWONLY

#define TOKEN_READ(scanner, token) plTokenRead(scanner, token)

int
plLookaheadStoreNoLog(plLexicalScanner *scanner, const plLexicalToken *token);
#define LOOKAHEAD_STORE(scanner, token) plLookaheadStoreNoLog(scanner, token)

#else

int
plTokenReadLog(const char *file_name, const char *function_name, unsigned int line_no,
               plLexicalScanner *scanner, plLexicalToken *token);
#define TOKEN_READ(scanner, token)      plTokenReadLog(__FILE__, __func__, __LINE__, scanner, token)

int
plLookaheadStoreLog(const char *file_name, const char *function_name, unsigned int line_no,
                    plLexicalScanner *scanner, const plLexicalToken *token);
#define LOOKAHEAD_STORE(scanner, token) plLookaheadStoreLog(__FILE__, __func__, __LINE__, scanner, token)

#endif  // LL_USE == VASQ_LL_RAWONLY

void
plTokenCleanup(plLexicalToken *token, plNameTable *table);

const char *
plLexicalMarkerName(int marker);

const char *
plStripLineBeginning(const char *line);

#if LL_USE == VASQ_LL_RAWONLY

#define COMPILER_ERROR(format, ...)                                                              \
    do {                                                                                         \
        VASQ_RAWLOG("%s:%u: " format "\n", scanner->file_name, scanner->line_no, ##__VA_ARGS__); \
        VASQ_RAWLOG("\t%s\n", plStripLineBeginning(scanner->buffer));                            \
    } while (0)

#else

#define COMPILER_ERROR(format, ...)                                                       \
    do {                                                                                  \
        VASQ_ERROR("%s:%u " format, scanner->file_name, scanner->line_no, ##__VA_ARGS__); \
        VASQ_ERROR("%s", plStripLineBeginning(scanner->buffer));                          \
    } while (0)

#endif  // LL_USE == VASQ_LL_RAWONLY
