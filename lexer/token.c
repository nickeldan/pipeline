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

void grabNextToken(char *text, plToken *token) {
	static size_t idx=0;
	static ssize_t textLen=-1;
	static size_t lineNo=0;

	if ( !text ) {
		idx=0;
		lineNo=0;
		textLen=-1;
		return;
	}
	if ( textLen == -1 ) {
		textLen=strlen(text);
	}

	token->lineNo=lineNo;

	look_for_token:
	if ( idx == textLen ) {
		token->marker=PL_MARKER_EOF;
	}
	else if ( isWhitespace(text[idx]) ) {
		token->marker=PL_MARKER_WHITESPACE;
		for(; idx<textLen && isWhitespace(text[idx]); idx++) {
			if ( text[idx] == '\n' ) {
				lineNo++;
			}
		}
	}
		else if ( text[idx] == ';' ) {
		token->marker=PL_MARKER_SEMICOLON;
		idx++;
	}
	else if ( text[idx] == '(' ) {
		token->marker=PL_MARKER_OPEN_PARENS;
		idx++;
	}
	else if ( text[idx] == ')' ) {
		token->marker=PL_MARKER_CLOSE_PARENS;
		idx++;
	}
	else if ( text[idx] == '{' ) {
		token->marker=PL_MARKER_OPEN_BRACE;
		idx++;
	}
	else if ( text[idx] == '}' ) {
		token->marker=PL_MARKER_CLOSE_BRACE;
		idx++;
	}
	else if ( text[idx] == '[' ) {
		token->marker=PL_MARKER_OPEN_BRACKET;
		idx++;
	}
	else if ( text[idx] == ']' ) {
		token->marker=PL_MARKER_CLOSE_BRACKET;
		idx++;
	}
	else if ( text[idx] == '.' ) {
		token->marker=PL_MARKER_PERIOD;
		idx++;
	}
	else if ( text[idx] == '#' ) {
		token->marker=PL_MARKER_OPTION;
		token->value.option=PL_OPTION_FORCE_TYPE;
		idx++;
	}
	else if ( text[idx] == '@' ) {
		token->marker=PL_MARKER_OPTION;
		token->value.option=PL_OPTION_MAP;
		idx++;
	}
	else if ( text[idx] == ':' ) {
		token->marker=PL_MARKER_COLON;
		idx++;
	}
	else if ( text[idx] == ',' ) {
		token->marker=PL_MARKER_COMMA;
		idx++;
	}
	else if ( text[idx] == '?' ) {
		token->marker=PL_MARKER_QUESTION;
		idx++;
	}
	else if ( strnEq(text+idx,"source",6) && !isVarChar(text[idx+6]) ) {
		token->marker=PL_MARKER_SOURCE;
		idx+=6;
	}
	else if ( strnEq(text+idx,"pipe",4) && !isVarChar(text[idx+4]) ) {
		token->marker=PL_MARKER_PIPE;
		idx+=4;
	}
	else if ( strnEq(text+idx,"sink",4) && !isVarChar(text[idx+4]) ) {
		token->marker=PL_MARKER_SINK;
		idx+=4;
	}
	else if ( strnEq(text+idx,"pred",4) && !isVarChar(text[idx+4]) ) {
		token->marker=PL_MARKER_PRED;
		idx+=4;
	}
	else if ( strnEq(text+idx,"import",6) && !isVarChar(text[idx+6]) ) {
		token->marker=PL_MARKER_IMPORT;
		idx+=7;
	}
	else if ( strnEq(text+idx,"prod",4) && !isVarChar(text[idx+4]) ) {
		token->marker=PL_MARKER_PROD;
		idx+=4;
	}
	else if ( strnEq(text+idx,"drop",4) && !isVarChar(text[idx+4]) ) {
		token->marker=PL_MARKER_DROP;
		idx+=4;
	}
	else if ( strnEq(text+idx,"end",3) && !isVarChar(text[idx+3]) ) {
		token->marker=PL_MARKER_END;
		idx+=3;
	}
	else if ( strnEq(text+idx,"local",5) && !isVarChar(text[idx+5]) ) {
		token->marker=PL_MARKER_LOCAL;
		idx+=5;
	}
	else if ( strnEq(text+idx,"detour",6) && !isVarChar(text[idx+6]) ) {
		token->marker=PL_MARKER_DETOUR;
		idx+=6;
	}
	else if ( strnEq(text+idx,"verify",6) && !isVarChar(text[idx+6]) ) {
		token->marker=PL_MARKER_VERIFY;
		idx+=6;
	}
	else if ( strnEq(text+idx,"while",5) && !isVarChar(text[idx+5]) ) {
		token->marker=PL_MARKER_WHILE;
		idx+=5;
	}
	else if ( strnEq(text+idx,"break",5) && !isVarChar(text[idx+5]) ) {
		token->marker=PL_MARKER_BREAK;
		idx+=5;
	}
	else if ( strnEq(text+idx,"continue",8) && !isVarChar(text[idx+8]) ) {
		token->marker=PL_MARKER_CONTINUE;
		idx+=8;
	}
	else if ( strnEq(text+idx,"if",2) && !isVarChar(text[idx+2]) ) {
		token->marker=PL_MARKER_IF;
		idx+=2;
	}
	else if ( strnEq(text+idx,"eif",3) && !isVarChar(text[idx+3]) ) {
		token->marker=PL_MARKER_EIF;
		idx+=3;
	}
	else if ( strnEq(text+idx,"else",4) && !isVarChar(text[idx+4]) ) {
		token->marker=PL_MARKER_ELSE;
		idx+=4;
	}
	else if ( strnEq(text+idx,"not",3) && !isVarChar(text[idx+3]) ) {
		token->marker=PL_MARKER_NOT;
		idx+=3;
	}
	else if ( strnEq(text+idx,"is",2) && !isVarChar(text[idx+2]) ) {
		token->marker=PL_MARKER_IS;
		idx+=2;
	}
	else if ( strnEq(text+idx,"and",3) && !isVarChar(text[idx+3]) ) {
		token->marker=PL_MARKER_LOGICAL;
		token->value.logical=PL_LOGICAL_AND;
		idx+=3;
	}
	else if ( strnEq(text+idx,"or",2) && !isVarChar(text[idx+2]) ) {
		token->marker=PL_MARKER_LOGICAL;
		token->value.logical=PL_LOGICAL_OR;
		idx+=2;
	}
	else if ( strnEq(text+idx,"null",4) && !isVarChar(text[idx+4]) ) {
		token->marker=PL_MARKER_LITERAL;
		token->value.object.value.staticValue=PL_STATIC_NULL;
		token->value.object.type=PL_TYPE_NULL;
		idx+=4;
	}
	else if ( strnEq(text+idx,"true",4) && !isVarChar(text[idx+4]) ) {
		token->marker=PL_MARKER_LITERAL;
		token->value.object.value.staticValue=PL_STATIC_TRUE;
		token->value.object.type=PL_TYPE_BOOL;
		idx+=4;
	}
	else if ( strnEq(text+idx,"false",4) && !isVarChar(text[idx+4]) ) {
		token->marker=PL_MARKER_LITERAL;
		token->value.object.value.staticValue=PL_STATIC_FALSE;
		token->value.object.type=PL_TYPE_BOOL;
		idx+=4;
	}
	else if ( strnEq(text+idx,"->",2) ) {
		token->marker=PL_MARKER_ARROW;
		idx+=2;
	}
	else if ( text[idx] == '+' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.op=PL_OPERATOR_PLUS;
		idx++;
	}
	else if ( text[idx] == '-' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.op=PL_OPERATOR_MINUS;
		idx++;
	}
	else if ( text[idx] == '*' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.op=PL_OPERATOR_MULTIPLY;
		idx++;
	}
	else if ( text[idx] == '/' && text[idx+1] != '*' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.op=PL_OPERATOR_DIVIDE;
		idx++;
	}
	else if ( text[idx] == '%' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.op=PL_OPERATOR_MODULO;
		idx++;
	}
	else if ( text[idx] == '^' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.op=PL_OPERATOR_XOR;
		idx++;
	}
	else if ( text[idx] == '&' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.op=PL_OPERATOR_MASK;
		idx++;
	}
	else if ( text[idx] == '|' ) {
		token->marker=PL_MARKER_OPERATOR;
		token->value.op=PL_OPERATOR_OR;
		idx++;
	}
	else if ( text[idx] == '<' ) {
		if ( text[idx+1] == '<' ) {
			token->marker=PL_MARKER_OPERATOR;
			token->value.op=PL_OPERATOR_LSHIFT;
			idx+=2;
		}
		else {
			token->marker=PL_MARKER_COMPARISON;
			if ( text[idx+1] == '=' ) {
				token->value.comparison=PL_COMPARISON_LESS_THAN_EQ;
				idx+=2;
			}
			else {
				token->value.comparison=PL_COMPARISON_LESS_THAN;
				idx++;
			}
		}
	}
	else if ( text[idx] == '>' ) {
		if ( text[idx+1] == '>' ) {
			token->marker=PL_MARKER_OPERATOR;
			token->value.op=PL_OPERATOR_RSHIFT;
			idx+=2;
		}
		else {
			token->marker=PL_MARKER_COMPARISON;
			if ( text[idx+1] == '=' ) {
				token->value.comparison=PL_COMPARISON_GREATER_THAN_EQ;
				idx+=2;
			}
			else {
				token->value.comparison=PL_COMPARISON_GREATER_THAN;
				idx++;
			}
		}
	}
	else if ( text[idx] == '=' ) {
		if ( text[idx+1] == '=' ) {
			token->marker=PL_MARKER_COMPARISON;
			token->value.comparison=PL_COMPARISON_EQUALS;
			idx+=2;
		}
		else {
			token->marker=PL_MARKER_ASSIGNMENT;
			idx++;
		}
	}
	else if ( strnEq(text+idx,"!=",3) == 0 ) {
		token->marker=PL_MARKER_COMPARISON;
		token->value.comparison=PL_COMPARISON_NOT_EQUALS;
		idx+=2;
	}
	else if ( isAlpha(text[idx]) ) {
		size_t idx2;
		char c;
		token->marker=PL_MARKER_NAME;
		for (idx2=idx+1; idx2<textLen && isVarChar(text[idx2]); idx2++);
		c=text[idx2];
		text[idx2]='\0';
		token->value.name=strdup(text+idx);
		text[idx2]=c;
		idx=idx2;
	}
	else if ( isNumeric(text[idx]) ) {
		size_t idx2;
		char c;
		token->marker=PL_MARKER_LITERAL;
		for (idx2=idx+1; idx2<textLen && isNumeric(text[idx2]); idx2++);
		if ( text[idx2] == '.' ) {
			token->value.object.type=PL_TYPE_FLOAT;
			for(idx2++; idx2<textLen && isNumeric(text[idx2]); idx2++);
			c=text[idx2];
			text[idx2]='\0';
			errno=0;
			token->value.object.value.decimal=atof(text+idx);
		}
		else {
			token->value.object.type=PL_TYPE_INT;
			c=text[idx2];
			text[idx2]='\0';
			errno=0;
			token->value.object.value.integer=atol(text+idx);
		}
		if ( errno != 0 ) {
			token->marker=PL_MARKER_INVALID_LITERAL;
			token->value.name=strdup(text+idx);
		}
		text[idx2]=c;
	}
	else if ( text[idx] == '/' ) {
		if ( text[idx+1] == '/' ) {
			while ( idx < textLen && text[idx] != '\n' ) {
				idx++;
			}
			goto look_for_token;
		}
		else if ( text[idx+1] == '*' ) {
			idx+=2;
			while ( idx < textLen ) {
				if ( strnEq(text+idx,"*/",2) ) {
					idx+=2;
					goto look_for_token;
				}
				if ( text[idx] == '\n' ) {
					lineNo++;
				}
			}
			token->marker=PL_MARKER_UNCLOSED_COMMENT_BLOCK;
		}
	}
	else {
		token->marker=PL_MARKER_UNKNOWN;
	}
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
	if ( c == 9 || c == 10 || c == 13 || c == 32 ) {
		return TRUE;
	}
	return FALSE;
}

static bool isAlpha(char c) {
	if ( c >= 97 && c < 123 ) { // lower-case
		return TRUE;
	}
	if ( c >= 65 && c < 91 ) { // upper-case
		return TRUE;
	}
	return FALSE;	
}

static bool isNumeric(char c) {
	if ( c >= 48 && c < 58 ) {
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
	if ( c == '_' ) {
		return TRUE;
	}
	return isAlnum(c);
}