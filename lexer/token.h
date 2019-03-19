#ifndef __PIPELINE_TOKEN_H__
#define __PIPELINE_TOKEN_H__

#include <sys/types.h>

#include "../pipeline/pipeline.h"

typedef enum {
	PL_MARKER_READ_FAILURE = 0,
	PL_MARKER_UNCLOSED_COMMENT_BLOCK,
	PL_MARKER_INVALID_LITERAL,
	PL_MARKER_NAME_TOO_LONG,
	PL_MARKER_UNKNOWN,
	PL_MARKER_EOF,
	PL_MARKER_SOURCE,
	PL_MARKER_PIPE,
	PL_MARKER_SINK,
	PL_MARKER_PRED,
	PL_MARKER_IMPORT,
	PL_MARKER_PROD,
	PL_MARKER_DROP,
	PL_MARKER_END,
	PL_MARKER_LOCAL,
	PL_MARKER_DETOUR,
	PL_MARKER_VERIFY,
	PL_MARKER_WHILE,
	PL_MARKER_BREAK,
	PL_MARKER_CONTINUE,
	PL_MARKER_IF,
	PL_MARKER_EIF,
	PL_MARKER_ELSE,
	PL_MARKER_NOT,
	PL_MARKER_IS,
	PL_MARKER_LOGICAL,
	PL_MARKER_OPERATOR,
	PL_MARKER_COMPARISON,
	PL_MARKER_ASSIGNMENT,
	PL_MARKER_NAME,
	PL_MARKER_LITERAL,
	PL_MARKER_BLANK,
	PL_MARKER_WHITESPACE,
	PL_MARKER_SEMICOLON,
	PL_MARKER_COLON,
	PL_MARKER_PERIOD,
	PL_MARKER_COMMA,
	PL_MARKER_QUESTION,
	PL_MARKER_OPEN_PARENS,
	PL_MARKER_CLOSE_PARENS,
	PL_MARKER_OPEN_BRACE,
	PL_MARKER_CLOSE_BRACE,
	PL_MARKER_OPEN_BRACKET,
	PL_MARKER_CLOSE_BRACKET,
	PL_MARKER_ARROW,
	PL_MARKER_OPTION,
} plToken_marker;

#define GOOD_MARKER(marker) (marker > PL_MARKER_EOF)

typedef enum {
	PL_LOGICAL_AND = 0,
	PL_LOGICAL_OR,
} plToken_logical;

typedef enum {
	PL_OPERATOR_PLUS = 0,
	PL_OPERATOR_MINUS,
	PL_OPERATOR_MULTIPLY,
	PL_OPERATOR_DIVIDE,
	PL_OPERATOR_MODULO,
	PL_OPERATOR_XOR,
	PL_OPERATOR_MASK,
	PL_OPERATOR_OR,
	PL_OPERATOR_LSHIFT,
	PL_OPERATOR_RSHIFT,
} plToken_operator;

typedef enum {
	PL_COMPARISON_EQUALS = 0,
	PL_COMPARISON_NOT_EQUALS,
	PL_COMPARISON_LESS_THAN,
	PL_COMPARISON_LESS_THAN_EQ,
	PL_COMPARISON_GREATER_THAN,
	PL_COMPARISON_GREATER_THAN_EQ,
} plToken_comparison;

typedef enum {
	PL_OPTION_FORCE_TYPE = 0,
	PL_OPTION_MAP,
} plToken_option;

typedef struct {
	union {
		char *name;
		plToken_logical logical;
		plToken_operator op;
		plToken_comparison comparison;
		plToken_option option;
		plObject object;
	} value;
	const char *fileName;
	off_t lineNo;
	plToken_marker marker;
} plToken;

#define PL_READER_BUFFER_SIZE 200

typedef struct {
	const char *path;
	char *idx;
	off_t lineNo;
	int fd, size;
	char text[PL_READER_BUFFER_SIZE];
} plFileReader;

bool initReader(plFileReader *reader, const char *path);
void closeReader(plFileReader *reader);
void grabNextToken(plFileReader *reader, plToken *token);

#endif // __PIPELINE_TOKEN_H__