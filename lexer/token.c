#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "token.h"

static bool updateReader(plFileReader *reader, int savedChars);
static bool isWhitespace(char c);
static bool isAlpha(char c);
static bool isNumeric(char c);
static bool isAlnum(char c);
static bool isVarChar(char c);

#ifdef DEBUG
#define DEBUG_MESSAGE(...) printf(__VA_ARGS__)
#else
#define DEBUG_MESSAGE(...) while (0)
#endif

bool initReader(plFileReader *reader, const char *path) {
	memset(reader,0,sizeof(plFileReader));
	reader->fd=open(path,O_RDONLY);
	if ( reader->fd == -1 ) {
		perror("open");
		return FALSE;
	}
	reader->path=path;
	return TRUE;
}

void closeReader(plFileReader *reader) {
	close(reader->fd);
}

void grabNextToken(plFileReader *reader, plToken *token) {
	look_for_token:
	if ( reader->idx >= reader->size ) {
		if ( !updateReader(reader,0) ) {
			token->marker=PL_MARKER_READ_FAILURE;
			return;
		}
		reader->idx=0;
	}
	else if ( reader->size - reader->idx <= PL_WORD_MAX_LENGTH ) {
		if ( !updateReader(reader,reader->size-reader->idx) ) {
			token->marker=PL_MARKER_READ_FAILURE;
			return;
		}
		reader->idx=0;
	}

	token->lineNo=reader->lineNo;
	token->fileName=reader->path;
	if ( reader->size == 0 ) {
		token->marker=PL_MARKER_EOF;
		DEBUG_MESSAGE("EOF\n");
		return;
	}
	else if ( isWhitespace(reader->text[reader->idx]) ) {
		token->marker=PL_MARKER_WHITESPACE;
		DEBUG_MESSAGE("WHITESPACE ");
		for(; isWhitespace(reader->text[reader->idx]); reader->idx++) {
			if ( reader->idx == reader->size ) {
				if ( !updateReader(reader,0) ) {
					token->marker=PL_MARKER_READ_FAILURE;
					return;
				}
				reader->idx=0;
			}
			if ( reader->text[reader->idx] == '\n' ) {
				reader->lineNo++;
				DEBUG_MESSAGE("\n");
			}
		}
	}
	else if ( reader->text[reader->idx] == ';' ) {
		token->marker=PL_MARKER_SEMICOLON;
		reader->idx++;
		DEBUG_MESSAGE("SEMICOLON ");
	}
	else if ( reader->text[reader->idx] == '(' ) {
		token->marker=PL_MARKER_OPEN_PARENS;
		reader->idx++;
		DEBUG_MESSAGE("OPEN_PARENS ");
	}
	else if ( reader->text[reader->idx] == ')' ) {
		token->marker=PL_MARKER_CLOSE_PARENS;
		reader->idx++;
		DEBUG_MESSAGE("CLOSE_PARENS ");
	}
	else if ( reader->text[reader->idx] == '{' ) {
		token->marker=PL_MARKER_OPEN_BRACE;
		reader->idx++;
		DEBUG_MESSAGE("OPEN_BRACE ");
	}
	else if ( reader->text[reader->idx] == '}' ) {
		token->marker=PL_MARKER_CLOSE_BRACE;
		reader->idx++;
		DEBUG_MESSAGE("CLOSE_BRACE ");
	}
	else if ( reader->text[reader->idx] == '[' ) {
		token->marker=PL_MARKER_OPEN_BRACKET;
		reader->idx++;
		DEBUG_MESSAGE("OPEN_BRACKET ");
	}
	else if ( reader->text[reader->idx] == ']' ) {
		token->marker=PL_MARKER_CLOSE_BRACKET;
		reader->idx++;
		DEBUG_MESSAGE("CLOSE_BRACKET ");
	}
	else if ( reader->text[reader->idx] == '.' && !isNumeric(reader->text[reader->idx]+1) ) {
		token->marker=PL_MARKER_PERIOD;
		reader->idx++;
		DEBUG_MESSAGE("PERIOD ");
	}
	else if ( reader->text[reader->idx] == '#' ) {
		token->marker=PL_MARKER_OPTION;
		token->value.option=PL_OPTION_FORCE_TYPE;
		reader->idx++;
		DEBUG_MESSAGE("OPTION ");
	}
	else if ( reader->text[reader->idx] == '@' ) {
		token->marker=PL_MARKER_OPTION;
		token->value.option=PL_OPTION_MAP;
		reader->idx++;
		DEBUG_MESSAGE("OPTION ");
	}
	else if ( reader->text[reader->idx] == ':' ) {
		token->marker=PL_MARKER_COLON;
		reader->idx++;
		DEBUG_MESSAGE("COLON ");
	}
	else if ( reader->text[reader->idx] == ',' ) {
		token->marker=PL_MARKER_COMMA;
		reader->idx++;
		DEBUG_MESSAGE("COMMA ");
	}
	else if ( reader->text[reader->idx] == '_' ) {
		token->marker=PL_MARKER_BLANK;
		reader->idx++;
		DEBUG_MESSAGE("BLANK ");
	}
	else if ( reader->text[reader->idx] == '?' ) {
		token->marker=PL_MARKER_QUESTION;
		reader->idx++;
		DEBUG_MESSAGE("QUESTION ");
	}
	else if ( strncmp(reader->text+reader->idx,"source",6) == 0 && !isVarChar(reader->text[reader->idx+6]) ) {
		token->marker=PL_MARKER_SOURCE;
		reader->idx+=6;
		DEBUG_MESSAGE("SOURCE ");
	}
	else if ( strncmp(reader->text+reader->idx,"pipe",4) == 0 && !isVarChar(reader->text[reader->idx+4]) ) {
		token->marker=PL_MARKER_PIPE;
		reader->idx+=4;
		DEBUG_MESSAGE("PIPE ");
	}
	else if ( strncmp(reader->text+reader->idx,"sink",4) == 0 && !isVarChar(reader->text[reader->idx+4]) ) {
		token->marker=PL_MARKER_SINK;
		reader->idx+=4;
		DEBUG_MESSAGE("SINK ");
	}
	else if ( strncmp(reader->text+reader->idx,"predicate",9) == 0 && !isVarChar(reader->text[reader->idx+9]) ) {
		token->marker=PL_MARKER_PRED;
		reader->idx+=9;
		DEBUG_MESSAGE("PRED ");
	}
	else if ( strncmp(reader->text+reader->idx,"import",6) == 0 && !isVarChar(reader->text[reader->idx+6]) ) {
		token->marker=PL_MARKER_IMPORT;
		reader->idx+=7;
		DEBUG_MESSAGE("IMPORT ");
	}
	else if ( strncmp(reader->text+reader->idx,"prod",4) == 0 && !isVarChar(reader->text[reader->idx+4]) ) {
		token->marker=PL_MARKER_PROD;
		reader->idx+=4;
		DEBUG_MESSAGE("PROD ");
	}
	else if ( strncmp(reader->text+reader->idx,"drop",4) == 0 && !isVarChar(reader->text[reader->idx+4]) ) {
		token->marker=PL_MARKER_DROP;
		reader->idx+=4;
		DEBUG_MESSAGE("DROP ");
	}
	else if ( strncmp(reader->text+reader->idx,"end",3) == 0 && !isVarChar(reader->text[reader->idx+3]) ) {
		token->marker=PL_MARKER_END;
		reader->idx+=3;
		DEBUG_MESSAGE("END ");
	}
	else if ( strncmp(reader->text+reader->idx,"local",5) == 0 && !isVarChar(reader->text[reader->idx+5]) ) {
		token->marker=PL_MARKER_LOCAL;
		reader->idx+=5;
		DEBUG_MESSAGE("LOCAL ");
	}
	else if ( strncmp(reader->text+reader->idx,"detour",6) == 0 && !isVarChar(reader->text[reader->idx+6]) ) {
		token->marker=PL_MARKER_DETOUR;
		reader->idx+=6;
		DEBUG_MESSAGE("DETOUR ");
	}
	else if ( strncmp(reader->text+reader->idx,"verify",6) == 0 && !isVarChar(reader->text[reader->idx+6]) ) {
		token->marker=PL_MARKER_VERIFY;
		reader->idx+=6;
		DEBUG_MESSAGE("VERIFY ");
	}
	else if ( strncmp(reader->text+reader->idx,"while",5) == 0 && !isVarChar(reader->text[reader->idx+5]) ) {
		token->marker=PL_MARKER_WHILE;
		reader->idx+=5;
		DEBUG_MESSAGE("WHILE ");
	}
	else if ( strncmp(reader->text+reader->idx,"break",5) == 0 && !isVarChar(reader->text[reader->idx+5]) ) {
		token->marker=PL_MARKER_BREAK;
		reader->idx+=5;
		DEBUG_MESSAGE("BREAK ");
	}
	else if ( strncmp(reader->text+reader->idx,"continue",8) == 0 && !isVarChar(reader->text[reader->idx+8]) ) {
		token->marker=PL_MARKER_CONTINUE;
		reader->idx+=8;
		DEBUG_MESSAGE("CONTINUE ");
	}
	else if ( strncmp(reader->text+reader->idx,"if",2) == 0 && !isVarChar(reader->text[reader->idx+2]) ) {
		token->marker=PL_MARKER_IF;
		reader->idx+=2;
		DEBUG_MESSAGE("IF ");
	}
	else if ( strncmp(reader->text+reader->idx,"eif",3) == 0 && !isVarChar(reader->text[reader->idx+3]) ) {
		token->marker=PL_MARKER_EIF;
		reader->idx+=3;
		DEBUG_MESSAGE("EIF ");
	}
	else if ( strncmp(reader->text+reader->idx,"else",4) == 0 && !isVarChar(reader->text[reader->idx+4]) ) {
		token->marker=PL_MARKER_ELSE;
		reader->idx+=4;
		DEBUG_MESSAGE("ELSE ");
	}
	else if ( strncmp(reader->text+reader->idx,"not",3) == 0 && !isVarChar(reader->text[reader->idx+3]) ) {
		token->marker=PL_MARKER_NOT;
		reader->idx+=3;
		DEBUG_MESSAGE("NOT ");
	}
	else if ( strncmp(reader->text+reader->idx,"is",2) == 0 && !isVarChar(reader->text[reader->idx+2]) ) {
		token->marker=PL_MARKER_IS;
		reader->idx+=2;
		DEBUG_MESSAGE("IS ");
	}
	else if ( strncmp(reader->text+reader->idx,"and",3) == 0 && !isVarChar(reader->text[reader->idx+3]) ) {
		token->marker=PL_MARKER_LOGICAL;
		token->value.logical=PL_LOGICAL_AND;
		reader->idx+=3;
		DEBUG_MESSAGE("LOGICAL ");
	}
	else if ( strncmp(reader->text+reader->idx,"or",2) == 0 && !isVarChar(reader->text[reader->idx+2]) ) {
		token->marker=PL_MARKER_LOGICAL;
		token->value.logical=PL_LOGICAL_OR;
		reader->idx+=2;
		DEBUG_MESSAGE("LOGICAL ");
	}
	else if ( strncmp(reader->text+reader->idx,"null",4) == 0 && !isVarChar(reader->text[reader->idx+4]) ) {
		token->marker=PL_MARKER_LITERAL;
		token->value.object.type=PL_TYPE_NULL;
		reader->idx+=4;
		DEBUG_MESSAGE("LITERAL ");
	}
	else if ( strncmp(reader->text+reader->idx,"true",4) == 0 && !isVarChar(reader->text[reader->idx+4]) ) {
		token->marker=PL_MARKER_LITERAL;
		token->value.object.value.staticValue=PL_STATIC_TRUE;
		token->value.object.type=PL_TYPE_BOOL;
		reader->idx+=4;
		DEBUG_MESSAGE("LITERAL ");
	}
	else if ( strncmp(reader->text+reader->idx,"false",5) == 0 && !isVarChar(reader->text[reader->idx+5]) ) {
		token->marker=PL_MARKER_LITERAL;
		token->value.object.value.staticValue=PL_STATIC_FALSE;
		token->value.object.type=PL_TYPE_BOOL;
		reader->idx+=4;
		DEBUG_MESSAGE("LITERAL ");
	}
	else if ( strncmp(reader->text+reader->idx,"->",2) == 0 ) {
		token->marker=PL_MARKER_ARROW;
		reader->idx+=2;
		DEBUG_MESSAGE("ARROW ");
	}
	else if ( reader->text[reader->idx] == '+' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.op=PL_OPERATOR_PLUS;
		reader->idx++;
		DEBUG_MESSAGE("OPERATOR ");
	}
	else if ( reader->text[reader->idx] == '-' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.op=PL_OPERATOR_MINUS;
		reader->idx++;
		DEBUG_MESSAGE("OPERATOR ");
	}
	else if ( reader->text[reader->idx] == '*' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.op=PL_OPERATOR_MULTIPLY;
		reader->idx++;
		DEBUG_MESSAGE("OPERATOR ");
	}
	else if ( reader->text[reader->idx] == '/' && reader->text[reader->idx+1] != '*' && reader->text[reader->idx+1] != '/' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.op=PL_OPERATOR_DIVIDE;
		reader->idx++;
		DEBUG_MESSAGE("OPERATOR ");
	}
	else if ( reader->text[reader->idx] == '%' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.op=PL_OPERATOR_MODULO;
		reader->idx++;
		DEBUG_MESSAGE("OPERATOR ");
	}
	else if ( reader->text[reader->idx] == '^' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.op=PL_OPERATOR_XOR;
		reader->idx++;
		DEBUG_MESSAGE("OPERATOR ");
	}
	else if ( reader->text[reader->idx] == '&' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.op=PL_OPERATOR_MASK;
		reader->idx++;
		DEBUG_MESSAGE("OPERATOR ");
	}
	else if ( reader->text[reader->idx] == '|' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.op=PL_OPERATOR_OR;
		reader->idx++;
		DEBUG_MESSAGE("OPERATOR ");
	}
	else if ( reader->text[reader->idx] == '<' ) {
		if ( reader->text[reader->idx+1] == '<' ) {
			token->marker=PL_MARKER_OPERATOR;
			token->value.op=PL_OPERATOR_LSHIFT;
			reader->idx+=2;
			DEBUG_MESSAGE("OPERATOR ");
		}
		else {
			token->marker=PL_MARKER_COMPARISON;
			if ( reader->text[reader->idx+1] == '=' ) {
				token->value.comparison=PL_COMPARISON_LESS_THAN_EQ;
				reader->idx+=2;
			}
			else {
				token->value.comparison=PL_COMPARISON_LESS_THAN;
				reader->idx++;
			}
			DEBUG_MESSAGE("COMPARISON ");
		}
	}
	else if ( reader->text[reader->idx] == '>' ) {
		if ( reader->text[reader->idx+1] == '>' ) {
			token->marker=PL_MARKER_OPERATOR;
			token->value.op=PL_OPERATOR_RSHIFT;
			reader->idx+=2;
			DEBUG_MESSAGE("OPERATOR ");
		}
		else {
			token->marker=PL_MARKER_COMPARISON;
			if ( reader->text[reader->idx+1] == '=' ) {
				token->value.comparison=PL_COMPARISON_GREATER_THAN_EQ;
				reader->idx+=2;
			}
			else {
				token->value.comparison=PL_COMPARISON_GREATER_THAN;
				reader->idx++;
			}
			DEBUG_MESSAGE("COMPARISON ");
		}
	}
	else if ( reader->text[reader->idx] == '=' ) {
		if ( reader->text[reader->idx+1] == '=' ) {
			token->marker=PL_MARKER_COMPARISON;
			token->value.comparison=PL_COMPARISON_EQUALS;
			reader->idx+=2;
			DEBUG_MESSAGE("COMPARISON ");
		}
		else {
			token->marker=PL_MARKER_ASSIGNMENT;
			reader->idx++;
			DEBUG_MESSAGE("ASSIGNMENT ");
		}
	}
	else if ( strncmp(reader->text+reader->idx,"!=",2) == 0 ) {
		token->marker=PL_MARKER_COMPARISON;
		token->value.comparison=PL_COMPARISON_NOT_EQUALS;
		reader->idx+=2;
		DEBUG_MESSAGE("COMPARISON) ");
	}
	else if ( isNumeric(reader->text[reader->idx]) ) {
		int idx2;
		char c;
		token->marker=PL_MARKER_LITERAL;
		for (idx2=reader->idx+1; isNumeric(reader->text[idx2]); idx2++);
		if ( idx2-reader->idx > PL_WORD_MAX_LENGTH ) {
			token->marker=PL_MARKER_INVALID_LITERAL;
			c=reader->text[idx2];
			reader->text[idx2]='\0';
			token->value.name=strdup(reader->text);
			reader->text[idx2]=c;
			reader->idx=idx2;
			DEBUG_MESSAGE("INVALID_LITERAL ");
			return;
		}
		if ( reader->text[idx2] == '.' ) {
			token->value.object.type=PL_TYPE_FLOAT;
			for(idx2++; isNumeric(reader->text[idx2]); idx2++);
			if ( idx2-reader->idx > PL_WORD_MAX_LENGTH ) {
				token->marker=PL_MARKER_INVALID_LITERAL;
				c=reader->text[idx2];
				reader->text[idx2]='\0';
				token->value.name=strdup(reader->text);
				reader->text[idx2]=c;
				reader->idx=idx2;
				DEBUG_MESSAGE("INVALID_LITERAL ");
				return;
			}
			c=reader->text[idx2];
			reader->text[idx2]='\0';
			errno=0;
			token->value.object.value.decimal=atof(reader->text+reader->idx);
		}
		else {
			token->value.object.type=PL_TYPE_INT;
			c=reader->text[idx2];
			reader->text[idx2]='\0';
			errno=0;
			token->value.object.value.integer=atol(reader->text+reader->idx);
		}
		if ( errno != 0 ) {
			token->marker=PL_MARKER_INVALID_LITERAL;
			token->value.name=strdup(reader->text+reader->idx);
			DEBUG_MESSAGE("INVALID_LITERAL ");
		}
		else {
			DEBUG_MESSAGE("LITERAL ");
		}
		reader->text[idx2]=c;
		reader->idx=idx2;
	}
	else if ( isAlpha(reader->text[reader->idx]) ) {
		size_t idx2;
		char c;
		token->marker=PL_MARKER_NAME;
		for (idx2=reader->idx+1; isVarChar(reader->text[idx2]); idx2++);
		if ( idx2-reader->idx > PL_WORD_MAX_LENGTH ) {
			token->marker=PL_MARKER_NAME_TOO_LONG;
			c=reader->text[idx2];
			reader->text[idx2]='\0';
			token->value.name=strdup(reader->text);
			reader->text[idx2]=c;
			reader->idx=idx2;
			DEBUG_MESSAGE("NAME_TOO_LONG ");
			return;
		}
		c=reader->text[idx2];
		reader->text[idx2]='\0';
		token->value.name=strdup(reader->text+reader->idx);
		reader->text[idx2]=c;
		reader->idx=idx2;
		DEBUG_MESSAGE("NAME ");
	}
	else if ( reader->text[reader->idx] == '/' ) {
		if ( reader->text[reader->idx+1] == '/' ) {
			read_more_for_single_line_comment:
			for(; reader->text[reader->idx] != '\n'; reader->idx++);
			if ( reader->idx == reader->size ) {
				if ( !updateReader(reader,0) ) {
					token->marker=PL_MARKER_READ_FAILURE;
					return;
				}
				if ( reader->size == 0 ) {
					return;
				}
				reader->idx=0;
				goto read_more_for_single_line_comment;
			}
		}
		else {
			reader->idx+=2;
			read_more_for_multi_line_comment:
			for (; reader->text[reader->idx]!='*'; reader->idx++) {
				if ( reader->text[reader->idx] == '\n' ) {
					reader->lineNo++;
				}
				if ( reader->idx+1 == reader->size ) {
					if ( !updateReader(reader,0) ) {
						token->marker=PL_MARKER_READ_FAILURE;
						return;
					}
					if ( reader->size == 0 ) {
						token->marker=PL_MARKER_UNCLOSED_COMMENT_BLOCK;
						DEBUG_MESSAGE("UNCLOSED_COMMENT_BLOCK(line %lli)\n", token->lineNo);
						return;
					}
					reader->idx=0;
				}
			}
			reader->idx++;
			if ( reader->text[reader->idx] != '/' ) {
				goto read_more_for_multi_line_comment;
			}
			reader->idx++;
		}
		goto look_for_token;
	}
	else {
		token->marker=PL_MARKER_UNKNOWN;
		DEBUG_MESSAGE("UNKNOWN(0x%02x, line %lli) ", reader->text[reader->idx], reader->lineNo);
		reader->idx++;
	}
}

static bool updateReader(plFileReader *reader, int savedChars) {
	if ( savedChars > 0 ) {
		memmove(reader->text,reader->text+reader->size-savedChars,savedChars);
	}
	reader->size=read(reader->fd,reader->text+savedChars,PL_READER_BUFFER_SIZE-savedChars);
	if ( reader->size == -1 ) {
		perror("read");
		return FALSE;
	}
	reader->size+=savedChars;
	if ( reader->size >= 0 ) {
		memset(reader->text+reader->size,0,PL_READER_BUFFER_SIZE-reader->size);
	}
	return TRUE;
}

static bool isWhitespace(char c) {
	if ( c == ' ' || c == '\t' || c == '\n' || c == '\r' ) {
		return TRUE;
	}
	return FALSE;
}

static bool isAlpha(char c) {
	if ( c >= 'a' && c <= 'z' ) {
		return TRUE;
	}
	if ( c >= 'A' && c <= 'Z' ) {
		return TRUE;
	}
	return FALSE;	
}

static bool isNumeric(char c) {
	if ( c >= '0' && c <= '9' ) {
		return TRUE;
	}
	return FALSE;
}

static bool isAlnum(char c) {
	if ( isAlpha(c) || isNumeric(c) ) {
		return TRUE;
	}
	return FALSE;
}

static bool isVarChar(char c) {
	if ( isAlnum(c) ) {
		return TRUE;
	}
	if ( c == '_' ) {
		return TRUE;
	}
	return FALSE;
}