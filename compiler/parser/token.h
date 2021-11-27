#ifndef PIPELINE_COMPILER_TOKEN_H
#define PIPELINE_COMPILER_TOKEN_H

#include <ctype.h>

// util header files
#include "util.h"

// vm header files
#include "object.h"

enum plLexicalMarker {
    PL_MARKER_USAGE = 256,
    PL_MARKER_READ_FAILURE,
    PL_MARKER_BAD_DATA,
    PL_MARKER_OUT_OF_MEMORY,
    PL_MARKER_EOF,

    PL_MARKER_INIT,
    PL_MARKER_ARROW,
    PL_MARKER_NAME,
    PL_MARKER_OBJECT,
    PL_MARKER_TYPE,
    PL_MARKER_SOURCE,
    PL_MARKER_PIPE,
    PL_MARKER_SINK,
    PL_MARKER_LOCAL,
    PL_MARKER_STRUCT,
    PL_MARKER_OPAQUE,
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
    PL_MARKER_EXPORT_ALL,
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
    PL_MARKER_CONTEXT,

    PL_MARKER_UNUSED,
};
#define VALID_MARKER(marker)    (((marker) > PL_MARKER_INIT && (marker) < PL_MARKER_UNUSED) || isprint(marker))
#define TERMINAL_MARKER(marker) ((marker) <= PL_MARKER_EOF)

enum plLexicalSubmarker {
    PL_SUBMARKER_NONE = 0,

    PL_SUBMARKER_OR,
    PL_SUBMARKER_AND,
    PL_SUBMARKER_ANY,
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
    PL_SUBMARKER_FUNC_DECL,
    PL_SUBMARKER_STORE,
    PL_SUBMARKER_ATTACH,
    PL_SUBMARKER_CONTEXT,
};

typedef struct plLexicalLocation {
    unsigned int line_no;
    unsigned int column_no;
} plLexicalLocation;

typedef struct plLexicalTokenHeader {
    plLexicalLocation location;
    int marker;
    int submarker;
} plLexicalTokenHeader;

typedef union plLexicalTokenData {
    const char *name;
    plObjectHandle handle;
} plLexicalTokenData;

typedef struct plLexicalToken {
    plLexicalTokenHeader header;
    plLexicalTokenData data;
} plLexicalToken;

const char *
plLexicalMarkerName(int marker);

#endif  // PIPELINE_COMPILER_TOKEN_H
