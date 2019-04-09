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
	uint8_t len;
	plToken_marker marker;
};

static bool updateReader(plFileReader *reader, int savedChars);
static bool isWhitespace(char c);
static bool isAlpha(char c);
static bool isNumeric(char c);
static bool isHex(char c);
static bool isAlnum(char c);
static bool isVarChar(char c);

#define PL_WORD_MAX_LENGTH 75

static const struct plToken_keyword keywords[]={
	{"source",6,PL_MARKER_SOURCE},
	{"pipe",4,PL_MARKER_SOURCE},
	{"sink",4,PL_MARKER_SINK},
	{"pred",4,PL_MARKER_PRED},
	{"struct",6,PL_MARKER_STRUCT},
	{"prod",4,PL_MARKER_PROD},
	{"drop",4,PL_MARKER_DROP},
	{"end",3,PL_MARKER_END},
	{"while",5,PL_MARKER_WHILE},
	{"not",3,PL_MARKER_NOT},
	{"and",3,PL_MARKER_LOGICAL},
	{"or",2,PL_MARKER_LOGICAL},
	{"true",4,PL_MARKER_LITERAL},
	{"false",5,PL_MARKER_LITERAL},
	{"null",4,PL_MARKER_LITERAL},
	{"sent",4,PL_MARKER_LITERAL},
	{"if",2,PL_MARKER_IF},
	{"eif",3,PL_MARKER_EIF},
	{"else",4,PL_MARKER_ELSE},
	{"cont",4,PL_MARKER_CONTINUE},
	{"break",5,PL_MARKER_BREAK},
	{"verify",6,PL_MARKER_VERIFY},
	{"local",5,PL_MARKER_LOCAL},
	{"detour",6,PL_MARKER_DETOUR},
	{"import",6,PL_MARKER_IMPORT},
	{"export",6,PL_MARKER_EXPORT},
	{"is",2,PL_MARKER_IS},
};
#define NUM_KEYWORDS (sizeof(keywords)/sizeof(struct plToken_keyword))

bool initReader(plFileReader *reader, const char *path) {
	reader->fd=open(path,O_RDONLY);
	if ( reader->fd == -1 ) {
		perror("open");
		return FALSE;
	}

	reader->path=path;
	reader->lineNo=1;
	reader->size=0;
	memset(reader->text,0,sizeof(PL_READER_BUFFER_SIZE));
	reader->idx=reader->text;

	return TRUE;
}

void closeReader(plFileReader *reader) {
	close(reader->fd);
}

void grabNextToken(plFileReader *reader, plToken *token) {
	ssize_t remainingBuffer;
	char firstChar;

	token->fileName=reader->path;

	look_for_token:

	remainingBuffer=reader->text+reader->size-reader->idx;
	if ( remainingBuffer <= PL_WORD_MAX_LENGTH && !updateReader(reader,remainingBuffer) ) {
		token->marker=PL_MARKER_READ_FAILURE;
		return;
	}

	token->lineNo=reader->lineNo;
	firstChar=reader->idx[0];
	if ( reader->size == 0 ) {
		token->marker=PL_MARKER_EOF;
	}
	else if ( isWhitespace(firstChar) ) {
		token->marker=PL_MARKER_WHITESPACE;
		read_more_for_whitespace:
		for (; isWhitespace(reader->idx[0]); reader->idx++) {
			if ( reader->idx[0] == '\n' ) {
				reader->lineNo++;
			}
		}
		if ( reader->idx == reader->text + reader->size ) {
			if ( !updateReader(reader,0) ) {
				token->marker=PL_MARKER_READ_FAILURE;
			}
			else if ( reader->size > 0 ) {
				goto read_more_for_whitespace;
			}
		}
	}
	else if ( isAlpha(firstChar) ) {
		int idx2;
		char c;
		for (int k=0; k<NUM_KEYWORDS; k++) {
			if ( strncmp(reader->idx,keywords[k].name,keywords[k].len) == 0 && !isVarChar(reader->idx[keywords[k].len]) ) {
				token->marker=keywords[k].marker;
				if ( token->marker == PL_MARKER_LOGICAL ) {
					token->value.submarker=( firstChar == 'a' )? PL_SUBMARKER_AND : PL_SUBMARKER_OR;
				}
				else if ( token->marker == PL_MARKER_LITERAL ) {
					switch ( firstChar ) {
						case 't':
						token->value.object.value.boolValue=PL_BOOL_TRUE;
						token->value.object.type=PL_TYPE_BOOL;
						break;
						case 'f':
						token->value.object.value.boolValue=PL_BOOL_FALSE;
						token->value.object.type=PL_TYPE_BOOL;
						break;
						case 'n':
						token->value.object.type=PL_TYPE_NULL;
						break;
						default:
						token->value.object.type=PL_TYPE_SENTINEL;
						break;
					}
				}
				reader->idx+=keywords[k].len;
				return;
			}
		}
		for (idx2=1; isVarChar(reader->idx[idx2]); idx2++);
		c=reader->idx[idx2];
		reader->idx[idx2]='\0';
		if ( idx2 > PL_WORD_MAX_LENGTH ) {
			token->marker=PL_MARKER_NAME_TOO_LONG;
		}
		else {
			token->marker=PL_MARKER_NAME;
		}
		token->value.name=strdup(reader->idx);
		reader->idx[idx2]=c;
		reader->idx+=idx2;
	}
	else if ( firstChar == ';' ) {
		token->marker=PL_MARKER_SEMICOLON;
		reader->idx++;
	}
	else if ( firstChar == '(' ) {
		token->marker=PL_MARKER_OPEN_PARENS;
		reader->idx++;
	}
	else if ( firstChar == ')' ) {
		token->marker=PL_MARKER_CLOSE_PARENS;
		reader->idx++;
	}
	else if ( firstChar == '{' ) {
		token->marker=PL_MARKER_OPEN_BRACE;
		reader->idx++;
	}
	else if ( firstChar == '}' ) {
		token->marker=PL_MARKER_CLOSE_BRACE;
		reader->idx++;
	}
	else if ( firstChar == '[' ) {
		token->marker=PL_MARKER_OPEN_BRACKET;
		reader->idx++;
	}
	else if ( firstChar == ']' ) {
		token->marker=PL_MARKER_CLOSE_BRACKET;
		reader->idx++;
	}
	else if ( firstChar == '.' ) {
		if ( isNumeric(reader->idx[1]) ) {
			int idx2;
			char c;
			for (idx2=2; isNumeric(reader->idx[idx2]); idx2++);
			c=reader->idx[idx2];
			reader->idx[idx2]='\0';
			if ( idx2 > PL_WORD_MAX_LENGTH ) {
				token->marker=PL_MARKER_INVALID_LITERAL;
				token->value.name=strdup(reader->idx);
			}
			else {
				errno=0;
				token->value.object.value.decimal=atof(reader->idx);
				if ( errno != 0 ) {
					token->marker=PL_MARKER_INVALID_LITERAL;
					token->value.name=strdup(reader->idx);
				}
				else {
					token->marker=PL_MARKER_LITERAL;
					token->value.object.type=PL_TYPE_FLOAT;
				}
			}
			reader->idx[idx2]=c;
			reader->idx+=idx2;
		}
		else {
			token->marker=PL_MARKER_PERIOD;
			reader->idx++;
		}
	}
	else if ( firstChar == '\'' ) {
		token->marker=PL_MARKER_QUOTE;
		reader->idx++;
	}
	else if ( firstChar == '"' ) {
		token->marker=PL_MARKER_DOUBLE_QUOTE;
		reader->idx++;
	}
	else if ( firstChar == '\\' ) {
		token->marker=PL_MARKER_ESCAPE;
		reader->idx++;
	}
	else if ( firstChar == '#' ) {
		token->marker=PL_MARKER_OPTION;
		token->value.submarker=PL_SUBMARKER_FORCE_TYPE;
		reader->idx++;
	}
	else if ( firstChar == '@' ) {
		token->marker=PL_MARKER_OPTION;
		token->value.submarker=PL_SUBMARKER_MAP;
		reader->idx++;
	}
	else if ( firstChar == ':' ) {
		token->marker=PL_MARKER_COLON;
		reader->idx++;
	}
	else if ( firstChar == ',' ) {
		token->marker=PL_MARKER_COMMA;
		reader->idx++;
	}
	else if ( firstChar == '_' ) {
		token->marker=PL_MARKER_BLANK;
		reader->idx++;
	}
	else if ( firstChar == '?' ) {
		token->marker=PL_MARKER_QUESTION;
		reader->idx++;
	}
	else if ( strncmp(reader->idx,"->",2) == 0 ) {
		token->marker=PL_MARKER_ARROW;
		reader->idx+=2;
	}
	else if ( firstChar == '+' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.submarker=PL_SUBMARKER_PLUS;
		reader->idx++;
	}
	else if ( firstChar == '-' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.submarker=PL_SUBMARKER_MINUS;
		reader->idx++;
	}
	else if ( firstChar == '*' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.submarker=PL_SUBMARKER_MULTIPLY;
		reader->idx++;
	}
	else if ( firstChar == '/' && reader->idx[1] != '*' && reader->idx[1] != '/' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.submarker=PL_SUBMARKER_DIVIDE;
		reader->idx++;
	}
	else if ( firstChar == '%' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.submarker=PL_SUBMARKER_MODULO;
		reader->idx++;
	}
	else if ( firstChar == '^' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.submarker=PL_SUBMARKER_XOR;
		reader->idx++;
	}
	else if ( firstChar == '&' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.submarker=PL_SUBMARKER_MASK;
		reader->idx++;
	}
	else if ( firstChar == '|' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.submarker=PL_SUBMARKER_BIT_OR;
		reader->idx++;
	}
	else if ( firstChar == '<' ) {
		if ( reader->idx[1] == '<' ) {
			token->marker=PL_MARKER_OPERATOR;
			token->value.submarker=PL_SUBMARKER_LSHIFT;
			reader->idx+=2;
		}
		else {
			token->marker=PL_MARKER_COMPARISON;
			if ( reader->idx[1] == '=' ) {
				token->value.submarker=PL_SUBMARKER_LESS_THAN_EQ;
				reader->idx+=2;
			}
			else {
				token->value.submarker=PL_SUBMARKER_LESS_THAN;
				reader->idx++;
			}
		}
	}
	else if ( firstChar == '>' ) {
		if ( reader->idx[1] == '>' ) {
			token->marker=PL_MARKER_OPERATOR;
			token->value.submarker=PL_SUBMARKER_RSHIFT;
			reader->idx+=2;
		}
		else {
			token->marker=PL_MARKER_COMPARISON;
			if ( reader->idx[1] == '=' ) {
				token->value.submarker=PL_SUBMARKER_GREATER_THAN_EQ;
				reader->idx+=2;
			}
			else {
				token->value.submarker=PL_SUBMARKER_GREATER_THAN;
				reader->idx++;
			}
		}
	}
	else if ( firstChar == '=' ) {
		if ( reader->idx[1] == '=' ) {
			token->marker=PL_MARKER_COMPARISON;
			token->value.submarker=PL_SUBMARKER_EQUALS;
			reader->idx+=2;
		}
		else {
			token->marker=PL_MARKER_ASSIGNMENT;
			reader->idx++;
		}
	}
	else if ( strncmp(reader->idx,"!=",2) == 0 ) {
		token->marker=PL_MARKER_COMPARISON;
		token->value.submarker=PL_SUBMARKER_NOT_EQUALS;
		reader->idx+=2;
	}
	else if ( isNumeric(firstChar) ) {
		int idx2;
		char c;
		for (idx2=1; isNumeric(reader->idx[idx2]); idx2++);
		if ( idx2 > PL_WORD_MAX_LENGTH ) {
			token->marker=PL_MARKER_INVALID_LITERAL;
			c=reader->idx[idx2];
			reader->idx[idx2]='\0';
			token->value.name=strdup(reader->idx);
			reader->idx[idx2]=c;
			reader->idx+=idx2;
			return;
		}
		if ( reader->idx[idx2] == '.' ) {
			token->value.object.type=PL_TYPE_FLOAT;
			for(idx2++; isNumeric(reader->idx[idx2]); idx2++);
			c=reader->idx[idx2];
			reader->idx[idx2]='\0';
			if ( idx2 > PL_WORD_MAX_LENGTH ) {
				token->marker=PL_MARKER_INVALID_LITERAL;
				token->value.name=strdup(reader->idx);
			}
			errno=0;
			token->value.object.type=PL_TYPE_FLOAT;
			token->value.object.value.decimal=atof(reader->idx);
		}
		else {
			c=reader->idx[idx2];
			reader->idx[idx2]='\0';
			errno=0;
			token->value.object.type=PL_TYPE_INT;
			token->value.object.value.integer=atol(reader->idx);
		}
		if ( errno != 0 ) {
			token->marker=PL_MARKER_INVALID_LITERAL;
			token->value.name=strdup(reader->idx);
		}
		else {
			token->marker=PL_MARKER_LITERAL;
		}
		reader->idx[idx2]=c;
		reader->idx+=idx2;
	}
	else if ( strncmp(reader->idx,"0x",2) == 0 ) {
		int idx2;
		char c;
		for (idx2=2; isHex(reader->idx[idx2]); idx2++);
		c=reader->idx[idx2];
		reader->idx[idx2]='\0';
		if ( idx2 == 2 || idx2 > PL_WORD_MAX_LENGTH ) {
			token->marker=PL_MARKER_INVALID_LITERAL;
			token->value.name=strdup(reader->idx);
		}
		else {
			errno=0;
			token->value.object.value.integer=strtoll(reader->idx,NULL,16);
			if ( errno != 0 ) {
				token->marker=PL_MARKER_INVALID_LITERAL;
			}
			else {
				token->marker=PL_MARKER_LITERAL;
				token->value.object.type=PL_TYPE_INT;
			}
		}
		reader->idx[idx2]=c;
		reader->idx+=idx2;
	}
	else if ( firstChar == '/' ) {
		if ( reader->idx[1] == '/' ) {
			read_more_for_single_line_comment:
			for(; reader->idx[0] != '\n'; reader->idx++);
			if ( reader->idx == reader->text + reader->size ) {
				if ( !updateReader(reader,0) ) {
					token->marker=PL_MARKER_READ_FAILURE;
					return;
				}
				if ( reader->size == 0 ) {
					return;
				}
				goto read_more_for_single_line_comment;
			}
		}
		else {
			reader->idx+=2;
			read_more_for_multi_line_comment:
			for (; reader->idx[0]!='*'; reader->idx++) {
				if ( reader->idx[0] == '\n' ) {
					reader->lineNo++;
				}
				if ( reader->idx+1 == reader->text + reader->size ) {
					if ( !updateReader(reader,0) ) {
						token->marker=PL_MARKER_READ_FAILURE;
						return;
					}
					if ( reader->size == 0 ) {
						token->marker=PL_MARKER_UNCLOSED_COMMENT_BLOCK;
						return;
					}
				}
			}
			reader->idx++;
			if ( reader->idx[0] != '/' ) {
				goto read_more_for_multi_line_comment;
			}
			reader->idx++;
		}
		goto look_for_token;
	}
	else {
		token->marker=PL_MARKER_UNKNOWN;
		token->value.character=reader->idx[0];
		reader->idx++;
	}
}

const char *tokenName(const plToken *token) {
	switch ( token->marker ) {
		case PL_MARKER_READ_FAILURE: return "READ_FAILURE";
		case PL_MARKER_INVALID_LITERAL: return "INVALID_LITERAL";
		case PL_MARKER_NAME_TOO_LONG: return "NAME_TOO_LONG";
		case PL_MARKER_EOF: return "EOF";
		case PL_MARKER_SOURCE: return "SOURCE";
		case PL_MARKER_PIPE: return "PIPE";
		case PL_MARKER_SINK: return "SINK";
		case PL_MARKER_PRED: return "PRED";
		case PL_MARKER_EXPORT: return "EXPORT";
		case PL_MARKER_IMPORT: return "IMPORT";
		case PL_MARKER_PROD: return "PROD";
		case PL_MARKER_DROP: return "DROP";
		case PL_MARKER_END: return "END";
		case PL_MARKER_LOCAL: return "LOCAL";
		case PL_MARKER_DETOUR: return "DETOUR";
		case PL_MARKER_VERIFY: return "VERIFY";
		case PL_MARKER_WHILE: return "WHILE";
		case PL_MARKER_BREAK: return "BREAK";
		case PL_MARKER_CONTINUE: return "CONTINUE";
		case PL_MARKER_IF: return "IF";
		case PL_MARKER_EIF: return "EIF";
		case PL_MARKER_ELSE: return "ELSE";
		case PL_MARKER_NOT: return "NOT";
		case PL_MARKER_IS: return "IS";
		case PL_MARKER_LOGICAL: return "LOGICAL";
		case PL_MARKER_OPERATOR: return "OPERATOR";
		case PL_MARKER_COMPARISON: return "COMPARISON";
		case PL_MARKER_ASSIGNMENT: return "ASSIGNMENT";
		case PL_MARKER_NAME: return "NAME";
		case PL_MARKER_LITERAL: return "LITERAL";
		case PL_MARKER_BLANK: return "BLANK";
		case PL_MARKER_WHITESPACE: return "WHITESPACE";
		case PL_MARKER_SEMICOLON: return "SEMICOLON";
		case PL_MARKER_COLON: return "COLON";
		case PL_MARKER_PERIOD: return "PERIOD";
		case PL_MARKER_COMMA: return "COMMA";
		case PL_MARKER_QUESTION: return "QUESTION";
		case PL_MARKER_OPEN_PARENS: return "OPEN_PARENS";
		case PL_MARKER_CLOSE_PARENS: return "CLOSE_PARENS";
		case PL_MARKER_OPEN_BRACE: return "OPEN_BRACE";
		case PL_MARKER_CLOSE_BRACE: return "CLOSE_BRACE";
		case PL_MARKER_OPEN_BRACKET: return "OPEN_BRACKET";
		case PL_MARKER_CLOSE_BRACKET: return "CLOSE_BRACKET";
		case PL_MARKER_ARROW: return "ARROW";
		case PL_MARKER_OPTION: return "OPTION";
		default: return "UNKNOWN";
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
	reader->idx=reader->text;
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