#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "token.h"

struct plToken_keyword {
	const char *name;
	int len;
	plToken_marker marker;
};

static bool updateReader(plFileReader *reader, int savedChars);
static bool isWhitespace(char c);
static bool isAlpha(char c);
static bool isNumeric(char c);
static bool isHex(char c);
static bool isAlnum(char c);
static bool isVarChar(char c);

#ifdef DEBUG
#define DEBUG_MESSAGE(...) printf(__VA_ARGS__)
#else
#define DEBUG_MESSAGE(...) while (0)
#endif

#define PL_WORD_MAX_LENGTH 75

static const struct plToken_keyword keywords[]={
	{"source",6,PL_MARKER_SOURCE},
	{"pipe",4,PL_MARKER_SOURCE},
	{"sink",4,PL_MARKER_SINK},
	{"pred",4,PL_MARKER_PRED},
	{"prod",4,PL_MARKER_PROD},
	{"drop",4,PL_MARKER_DROP},
	{"end",3,PL_MARKER_END},
	{"while",5,PL_MARKER_WHILE},
	{"not",3,PL_MARKER_NOT},
	{"and",3,PL_MARKER_LOGICAL},
	{"or",2,PL_MARKER_LOGICAL},
	{"null",4,PL_MARKER_LITERAL},
	{"true",4,PL_MARKER_LITERAL},
	{"false",4,PL_MARKER_LITERAL},
	{"if",2,PL_MARKER_IF},
	{"eif",3,PL_MARKER_EIF},
	{"else",4,PL_MARKER_ELSE},
	{"cont",4,PL_MARKER_CONTINUE},
	{"break",5,PL_MARKER_BREAK},
	{"verify",6,PL_MARKER_VERIFY},
	{"local",5,PL_MARKER_LOCAL},
	{"detour",6,PL_MARKER_DETOUR},
	{"import",6,PL_MARKER_IMPORT},
	{"is",2,PL_MARKER_IS},
};
#define NUM_KEYWORDS (sizeof(keywords)/sizeof(struct plToken_keyword))

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
	token->fileName=reader->path;
	look_for_token:
	if ( reader->size - reader->idx <= PL_WORD_MAX_LENGTH ) {
		if ( !updateReader(reader,reader->size-reader->idx) ) {
			token->marker=PL_MARKER_READ_FAILURE;
			return;
		}
		reader->idx=0;
	}

	token->lineNo=reader->lineNo;
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
	else if ( isAlpha(reader->text[reader->idx]) ) {
		int idx2;
		char c;
		for (int k=0; k<NUM_KEYWORDS; k++) {
			if ( strncmp(reader->text+reader->idx,keywords[k].name,keywords[k].len) == 0 && !isVarChar(reader->text[reader->idx+keywords[k].len]) ) {
				token->marker=keywords[k].marker;
				if ( token->marker == PL_MARKER_LOGICAL ) {
					token->value.logical=( reader->text[reader->idx] == 'a' )? PL_LOGICAL_AND : PL_LOGICAL_OR;
					DEBUG_MESSAGE("LOGICAL ");
				}
				else if ( token->marker == PL_MARKER_LITERAL ) {
					switch ( reader->text[reader->idx] ) {
						case 't':
						token->value.object.value.staticValue=PL_STATIC_TRUE;
						token->value.object.type=PL_TYPE_BOOL;
						break;
						case 'f':
						token->value.object.value.staticValue=PL_STATIC_FALSE;
						token->value.object.type=PL_TYPE_BOOL;
						break;
						default:
						token->value.object.type=PL_TYPE_NULL;
						break;
					}
					DEBUG_MESSAGE("LITERAL ");
				}
#ifdef DEBUG
				else {
					char upperName[15];
					for (int j=0; j<keywords[k].len; j++) {
						upperName[j]=keywords[k].name[j]-32; // Make character upper-case.
					}
					upperName[keywords[k].len]='\0';
					DEBUG_MESSAGE("%s ", upperName);
				}
#endif
				reader->idx+=keywords[k].len;
				return;
			}
		}
		for (idx2=reader->idx+1; isVarChar(reader->text[idx2]); idx2++);
		c=reader->text[idx2];
		reader->text[idx2]='\0';
		if ( idx2-reader->idx > PL_WORD_MAX_LENGTH ) {
			token->marker=PL_MARKER_NAME_TOO_LONG;
			DEBUG_MESSAGE("NAME_TOO_LONG ");
		}
		else {
			token->marker=PL_MARKER_NAME;
			DEBUG_MESSAGE("NAME ");
		}
		token->value.name=strdup(reader->text+reader->idx);
		reader->text[idx2]=c;
		reader->idx=idx2;
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
	else if ( reader->text[reader->idx] == '.' ) {
		if ( isNumeric(reader->text[reader->idx+1]) ) {
			int idx2;
			char c;
			for (idx2=reader->idx+2; isNumeric(reader->text[idx2]); idx2++);
			c=reader->text[idx2];
			reader->text[idx2]='\0';
			if ( idx2-reader->idx > PL_WORD_MAX_LENGTH ) {
				token->marker=PL_MARKER_INVALID_LITERAL;
				token->value.name=strdup(reader->text+reader->idx);
				DEBUG_MESSAGE("INVALID_LITERAL ");
			}
			else {
				errno=0;
				token->value.object.value.decimal=atof(reader->text+reader->idx);
				if ( errno != 0 ) {
					token->marker=PL_MARKER_INVALID_LITERAL;
					token->value.name=strdup(reader->text+reader->idx);
					DEBUG_MESSAGE("INVALID_LITERAL ");
				}
				else {
					token->marker=PL_MARKER_LITERAL;
					token->value.object.type=PL_TYPE_FLOAT;
					DEBUG_MESSAGE("LITERAL ");
				}
			}
			reader->idx=idx2;
			reader->text[idx2]=c;
		}
		else {
			token->marker=PL_MARKER_PERIOD;
			reader->idx++;
			DEBUG_MESSAGE("PERIOD ");
		}
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
			c=reader->text[idx2];
			reader->text[idx2]='\0';
			if ( idx2-reader->idx > PL_WORD_MAX_LENGTH ) {
				token->marker=PL_MARKER_INVALID_LITERAL;
				token->value.name=strdup(reader->text+reader->idx);
				DEBUG_MESSAGE("INVALID_LITERAL ");
			}
			errno=0;
			token->value.object.type=PL_TYPE_FLOAT;
			token->value.object.value.decimal=atof(reader->text+reader->idx);
		}
		else {
			c=reader->text[idx2];
			reader->text[idx2]='\0';
			errno=0;
			token->value.object.type=PL_TYPE_INT;
			token->value.object.value.integer=atol(reader->text+reader->idx);
		}
		if ( errno != 0 ) {
			token->marker=PL_MARKER_INVALID_LITERAL;
			token->value.name=strdup(reader->text+reader->idx);
			DEBUG_MESSAGE("INVALID_LITERAL ");
		}
		else {
			token->marker=PL_MARKER_LITERAL;
			DEBUG_MESSAGE("LITERAL ");
		}
		reader->text[idx2]=c;
		reader->idx=idx2;
	}
	else if ( strncmp(reader->text+reader->idx,"0x",2) == 0 ) {
		int idx2;
		char c;
		for (idx2=reader->idx+2; isHex(reader->text[idx2]); idx2++);
		c=reader->text[idx2];
		reader->text[idx2]='\0';
		if ( idx2 == reader->idx+2 || idx2-reader->idx > PL_WORD_MAX_LENGTH ) {
			token->marker=PL_MARKER_INVALID_LITERAL;
			token->value.name=strdup(reader->text+reader->idx);
			DEBUG_MESSAGE("INVALID_LITERAL ");
		}
		else {
			errno=0;
			token->value.object.value.integer=strtoll(reader->text+reader->idx,NULL,16);
			if ( errno != 0 ) {
				token->marker=PL_MARKER_INVALID_LITERAL;
				DEBUG_MESSAGE("INVALID_LITERAL ");
			}
			else {
				token->marker=PL_MARKER_LITERAL;
				token->value.object.type=PL_TYPE_INT;
				DEBUG_MESSAGE("LITERAL ");
			}
		}
		reader->text[idx2]=c;
		reader->idx=idx2;
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
						DEBUG_MESSAGE("UNCLOSED_COMMENT_BLOCK\n");
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
		DEBUG_MESSAGE("UNKNOWN(0x%02x) ", (uint8_t)(reader->text[reader->idx]));
		reader->idx++;
	}
}

static bool updateReader(plFileReader *reader, int savedChars) {
	if ( savedChars > 0 ) {
		memmove(reader->text,reader->text+reader->size-savedChars,savedChars);
	}
	else {
		savedChars=0;
	}
	reader->size=read(reader->fd,reader->text+savedChars,PL_READER_BUFFER_SIZE-savedChars);
	if ( reader->size == -1 ) {
		perror("read");
		return FALSE;
	}
	reader->size+=savedChars;
	memset(reader->text+reader->size,0,PL_READER_BUFFER_SIZE-reader->size);
	return TRUE;
}

static bool isWhitespace(char c) {
	return ( c == ' ' || c == '\t' || c == '\n' || c == '\r' );
}

static bool isAlpha(char c) {
	return ( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) );
}

static bool isNumeric(char c) {
	return ( c >= '0' && c <= '9' );
}

static bool isHex(char c) {
	return ( isNumeric(c) || ( c >= 'a' && c <= 'f' ) || ( c >= 'A' && c <= 'F' ) );
}

static bool isAlnum(char c) {
	return ( isAlpha(c) || isNumeric(c) );
}

static bool isVarChar(char c) {
	return ( isAlnum(c) || c == '_' );
}