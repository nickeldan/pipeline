#include <stdib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "../pipeline/util.h"
#include "../lexer/token.h"

typedef struct {
	plToken *tokens;
	size_t length, size;
} plTokenArray;

#define PL_DYNAMIC_TOKEN_ARRAY_MIN_SIZE 100

enum plParseFrameMarker {
	PL_PARSE_FRAME_SINK=0,
	PL_PARSE_FRAME_PIPE,
	PL_PARSE_FRAME_SINK,
	PL_PARSE_FRAME_LOCAL,
	PL_PARSE_FRAME_PRED,
	PL_PARSE_FRAME_STRUCT,
	PL_PARSE_FRAME_WHILE,
	PL_PARSE_FRAME_IF,
	PL_PARSE_FRAME_EXPRESSION,
};

typedef struct {
	void *data;
	uint8_t marker;
} plParseFrame;

typedef struct {
	plParseFrame *frames;
	size_t length, size, offset;
} plParseStack;

#define PL_PARSE_STACK_MIN_SIZE 30

static bool addTokenToArray(plTokenArray *array, const plToken *token);
static void clearArray(plTokenArray *array);
static bool pushToStack(plParseStack *stack, uint8_t marker);
static void popFromStack(plParseStack *stack);
static void clearStack(plParseStack *stack);
static plErrorCode recursivelyParseTokens(plTokenArray *array, size_t *offset, plParseStack *stack, plModule *module);

plErrorCode parseFile(const char *path, plModule *module) {
	plErrorCode ret=PL_ERROR_OK;
	plFileReader reader;
	plTokenArray array;
	plParseStack stack;
	size_t offset;

	if ( !initReader(&reader,path) ) {
		return PL_ERROR_FILE_ACCESS;
	}

	memset(&array,0,sizeof(plTokenArray));

	while ( TRUE ) {
		plToken token;

		grabNextToken(&reader,&token);

		if ( token.marker == PL_MARKER_EOF ) {
			break;
		}
		else if ( token.marker == PL_MARKER_READ_FAILURE ) {
			ret=PL_ERROR_FILE_READ;
			break;
		}
		else if ( token.marker == PL_MARKER_MALLOC_FAILURE ) {
			ret=PL_ERROR_MEMORY;
			break;
		}
		else if ( TERMINAL_MARKER(token.marker) ) {
			printf("%s: line %u\n", path, token.lineNo);
			printf("Lexical error: %s\n", tokenName(&token));

			ret=PL_ERROR_LEX;
			break;
		}

		if ( !addTokenToArray(&array,&token) ) {
			clearToken(&token);

			ret=PL_ERROR_MEMORY;
			break;
		}
	}

	closeReader(&reader);

	if ( ret != PL_ERROR_OK ) {
		clearArray(&array);

		return ret;
	}

	offset=0;
	memset(&stack,0,sizeof(plParseStack));

	ret=recursivelyParseTokens(&array,&offset,&stack,module);
	if ( ret != PL_ERROR_OK ) {
		clearArray(&array);

		return ret;
	}

	
}

static bool addTokenToArray(plTokenArray *array, const plToken *token) {
	if ( array->size == 0 ) {
		array->tokens=malloc(sizeof(plToken)*PL_DYNAMIC_TOKEN_ARRAY_MIN_SIZE);
		if ( !array->tokens ) {
			return FALSE;
		}
	}
	else if ( array->length == array->size ) {
		size_t newSize;
		plToken *success;

		newSize=array->size*5/4;
		success=realloc(array->token,sizeof(plToken)*newSize);
		if ( !success ) {
			return FALSE;
		}

		array->tokens=success;
		array->size=newSize;
	}

	memcpy(array->tokens+array->length,token,sizeof(plToken));
	array->length++;

	return TRUE;
}

static void clearArray(plTokenArray *array) {
	if ( array->tokens ) {
		for (size_t k=0; k<array->length; k++) {
			clearToken(array->tokens+k);
		}

		free(array->tokens);

		memset(array,0,sizeof(plTokenArray));
	}
}

static plErrorCode recursivelyParseTokens(plTokenArray *array, size_t *offset, plParseStack *stack, plModule *module) {

}