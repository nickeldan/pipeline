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
	PL_MARKER_EXPORT,
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
	PL_MARKER_QUOTE,
	PL_MARKER_DOUBLE_QUOTE,
	PL_MARKER_ESCAPE,
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

#define GOOD_MARKER(marker) (marker >= PL_MARKER_EOF)

typedef enum {
	PL_SUBMARKER_AND = 0,
	PL_SUBMARKER_OR,
	PL_SUBMARKER_PLUS,
	PL_SUBMARKER_MINUS,
	PL_SUBMARKER_MULTIPLY,
	PL_SUBMARKER_DIVIDE,
	PL_SUBMARKER_MODULO,
	PL_SUBMARKER_XOR,
	PL_SUBMARKER_MASK,
	PL_SUBMARKER_BIT_OR,
	PL_SUBMARKER_LSHIFT,
	PL_SUBMARKER_RSHIFT,
	PL_SUBMARKER_EQUALS,
	PL_SUBMARKER_NOT_EQUALS,
	PL_SUBMARKER_LESS_THAN,
	PL_SUBMARKER_LESS_THAN_EQ,
	PL_SUBMARKER_GREATER_THAN,
	PL_SUBMARKER_GREATER_THAN_EQ,
	PL_SUBMARKER_FORCE_TYPE,
	PL_SUBMARKER_MAP,
} plToken_submarker;

typedef struct {
	union {
		char *name;
		plToken_submarker submarker;
		plObject object;
		char character;
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
const char *tokenName(const plToken *token);

#endif // __PIPELINE_TOKEN_H__