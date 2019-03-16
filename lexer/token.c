#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "token.h"

static bool strnEq(const char *text1, const char *text2, size_t len);
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

void grabNextToken(plFileReader *reader, plToken *token) {
	look_for_token:
	if ( reader->idx == reader->textLen ) {
		token->marker=PL_MARKER_EOF;
		DEBUG_MESSAGE("EOF\n");
	}
	else if ( isWhitespace(reader->text[reader->idx]) ) {
		token->marker=PL_MARKER_WHITESPACE;
		DEBUG_MESSAGE("WHITESPACE ");
		for(; isWhitespace(reader->text[reader->idx]) && reader->idx<reader->textLen; reader->idx++) {
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
	else if ( reader->text[reader->idx] == '.' ) {
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
	else if ( strnEq(reader->text+reader->idx,"source",6) && !isVarChar(reader->text[reader->idx+6]) ) {
		token->marker=PL_MARKER_SOURCE;
		reader->idx+=6;
		DEBUG_MESSAGE("SOURCE ");
	}
	else if ( strnEq(reader->text+reader->idx,"pipe",4) && !isVarChar(reader->text[reader->idx+4]) ) {
		token->marker=PL_MARKER_PIPE;
		reader->idx+=4;
		DEBUG_MESSAGE("PIPE ");
	}
	else if ( strnEq(reader->text+reader->idx,"sink",4) && !isVarChar(reader->text[reader->idx+4]) ) {
		token->marker=PL_MARKER_SINK;
		reader->idx+=4;
		DEBUG_MESSAGE("SINK ");
	}
	else if ( strnEq(reader->text+reader->idx,"predicate",9) && !isVarChar(reader->text[reader->idx+9]) ) {
		token->marker=PL_MARKER_PRED;
		reader->idx+=9;
		DEBUG_MESSAGE("PRED ");
	}
	else if ( strnEq(reader->text+reader->idx,"import",6) && !isVarChar(reader->text[reader->idx+6]) ) {
		token->marker=PL_MARKER_IMPORT;
		reader->idx+=7;
		DEBUG_MESSAGE("IMPORT ");
	}
	else if ( strnEq(reader->text+reader->idx,"prod",4) && !isVarChar(reader->text[reader->idx+4]) ) {
		token->marker=PL_MARKER_PROD;
		reader->idx+=4;
		DEBUG_MESSAGE("PROD ");
	}
	else if ( strnEq(reader->text+reader->idx,"drop",4) && !isVarChar(reader->text[reader->idx+4]) ) {
		token->marker=PL_MARKER_DROP;
		reader->idx+=4;
		DEBUG_MESSAGE("DROP ");
	}
	else if ( strnEq(reader->text+reader->idx,"end",3) && !isVarChar(reader->text[reader->idx+3]) ) {
		token->marker=PL_MARKER_END;
		reader->idx+=3;
		DEBUG_MESSAGE("END ");
	}
	else if ( strnEq(reader->text+reader->idx,"local",5) && !isVarChar(reader->text[reader->idx+5]) ) {
		token->marker=PL_MARKER_LOCAL;
		reader->idx+=5;
		DEBUG_MESSAGE("LOCAL ");
	}
	else if ( strnEq(reader->text+reader->idx,"detour",6) && !isVarChar(reader->text[reader->idx+6]) ) {
		token->marker=PL_MARKER_DETOUR;
		reader->idx+=6;
		DEBUG_MESSAGE("DETOUR ");
	}
	else if ( strnEq(reader->text+reader->idx,"verify",6) && !isVarChar(reader->text[reader->idx+6]) ) {
		token->marker=PL_MARKER_VERIFY;
		reader->idx+=6;
		DEBUG_MESSAGE("VERIFY ");
	}
	else if ( strnEq(reader->text+reader->idx,"while",5) && !isVarChar(reader->text[reader->idx+5]) ) {
		token->marker=PL_MARKER_WHILE;
		reader->idx+=5;
		DEBUG_MESSAGE("WHILE ");
	}
	else if ( strnEq(reader->text+reader->idx,"break",5) && !isVarChar(reader->text[reader->idx+5]) ) {
		token->marker=PL_MARKER_BREAK;
		reader->idx+=5;
		DEBUG_MESSAGE("BREAK ");
	}
	else if ( strnEq(reader->text+reader->idx,"continue",8) && !isVarChar(reader->text[reader->idx+8]) ) {
		token->marker=PL_MARKER_CONTINUE;
		reader->idx+=8;
		DEBUG_MESSAGE("CONTINUE ");
	}
	else if ( strnEq(reader->text+reader->idx,"if",2) && !isVarChar(reader->text[reader->idx+2]) ) {
		token->marker=PL_MARKER_IF;
		reader->idx+=2;
		DEBUG_MESSAGE("IF ");
	}
	else if ( strnEq(reader->text+reader->idx,"eif",3) && !isVarChar(reader->text[reader->idx+3]) ) {
		token->marker=PL_MARKER_EIF;
		reader->idx+=3;
		DEBUG_MESSAGE("EIF ");
	}
	else if ( strnEq(reader->text+reader->idx,"else",4) && !isVarChar(reader->text[reader->idx+4]) ) {
		token->marker=PL_MARKER_ELSE;
		reader->idx+=4;
		DEBUG_MESSAGE("ELSE ");
	}
	else if ( strnEq(reader->text+reader->idx,"not",3) && !isVarChar(reader->text[reader->idx+3]) ) {
		token->marker=PL_MARKER_NOT;
		reader->idx+=3;
		DEBUG_MESSAGE("NOT ");
	}
	else if ( strnEq(reader->text+reader->idx,"is",2) && !isVarChar(reader->text[reader->idx+2]) ) {
		token->marker=PL_MARKER_IS;
		reader->idx+=2;
		DEBUG_MESSAGE("IS ");
	}
	else if ( strnEq(reader->text+reader->idx,"and",3) && !isVarChar(reader->text[reader->idx+3]) ) {
		token->marker=PL_MARKER_LOGICAL;
		token->value.logical=PL_LOGICAL_AND;
		reader->idx+=3;
		DEBUG_MESSAGE("LOGICAL ");
	}
	else if ( strnEq(reader->text+reader->idx,"or",2) && !isVarChar(reader->text[reader->idx+2]) ) {
		token->marker=PL_MARKER_LOGICAL;
		token->value.logical=PL_LOGICAL_OR;
		reader->idx+=2;
		DEBUG_MESSAGE("LOGICAL ");
	}
	else if ( strnEq(reader->text+reader->idx,"null",4) && !isVarChar(reader->text[reader->idx+4]) ) {
		token->marker=PL_MARKER_LITERAL;
		token->value.object.type=PL_TYPE_NULL;
		reader->idx+=4;
		DEBUG_MESSAGE("LITERAL ");
	}
	else if ( strnEq(reader->text+reader->idx,"true",4) && !isVarChar(reader->text[reader->idx+4]) ) {
		token->marker=PL_MARKER_LITERAL;
		token->value.object.value.staticValue=PL_STATIC_TRUE;
		token->value.object.type=PL_TYPE_BOOL;
		reader->idx+=4;
		DEBUG_MESSAGE("LITERAL ");
	}
	else if ( strnEq(reader->text+reader->idx,"false",4) && !isVarChar(reader->text[reader->idx+4]) ) {
		token->marker=PL_MARKER_LITERAL;
		token->value.object.value.staticValue=PL_STATIC_FALSE;
		token->value.object.type=PL_TYPE_BOOL;
		reader->idx+=4;
		DEBUG_MESSAGE("LITERAL ");
	}
	else if ( strnEq(reader->text+reader->idx,"->",2) ) {
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
	else if ( strnEq(reader->text+reader->idx,"!=",2) ) {
		token->marker=PL_MARKER_COMPARISON;
		token->value.comparison=PL_COMPARISON_NOT_EQUALS;
		reader->idx+=2;
		DEBUG_MESSAGE("COMPARISON) ");
	}
	else if ( isNumeric(reader->text[reader->idx]) ) {
		size_t idx2;
		char c;
		token->marker=PL_MARKER_LITERAL;
		for (idx2=reader->idx+1; idx2<reader->textLen && isNumeric(reader->text[idx2]); idx2++);
		if ( reader->text[idx2] == '.' ) {
			token->value.object.type=PL_TYPE_FLOAT;
			for(idx2++; isNumeric(reader->text[idx2]); idx2++);
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
		for (idx2=reader->idx+1; idx2<reader->textLen && isVarChar(reader->text[idx2]); idx2++);
		c=reader->text[idx2];
		reader->text[idx2]='\0';
		token->value.name=strdup(reader->text+reader->idx);
		reader->text[idx2]=c;
		reader->idx=idx2;
		DEBUG_MESSAGE("NAME ");
	}
	else if ( reader->text[reader->idx] == '/' ) {
		if ( reader->text[reader->idx+1] == '/' ) {
			for(; reader->idx<reader->textLen && reader->text[reader->idx] != '\n'; reader->idx++);
			goto look_for_token;
		}
		else if ( reader->text[reader->idx+1] == '*' ) {
			size_t origLineNo=reader->lineNo;
			for(reader->idx+=2; reader->idx<reader->textLen; reader->idx++) {
				if ( strnEq(reader->text+reader->idx,"*/",2) ) {
					reader->idx+=2;
					goto look_for_token;
				}
				if ( reader->text[reader->idx] == '\n' ) {
					reader->lineNo++;
				}
			}
			token->marker=PL_MARKER_UNCLOSED_COMMENT_BLOCK;
			DEBUG_MESSAGE("UNCLOSED_COMMENT_BLOCK(line %lu) ", origLineNo);
		}
	}
	else {
		token->marker=PL_MARKER_UNKNOWN;
		reader->idx++;
		DEBUG_MESSAGE("UNKNOWN ");
	}
	token->lineNo=reader->lineNo;
}

static bool strnEq(const char *text1, const char *text2, size_t len) {
	for (size_t k=0; k<len; k++) {
		if ( text1[k] != text2[k] ) {
			return FALSE;
		}
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