%option nodefault yylineno

%{
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "ast.h"
#include "marker.h"
#include "nameTable.h"
#include "util.h"
#include "object.h"

#include "parser.tab.h"

void yyerror(const char *format, ...);
extern int yylineno;

static plObject *lexResolveSimpleLiteral(char firstChar) __attribute__((pure));
static plObject *lexResolveIntLiteral(const char *text, size_t len);
static plObject *lexResoleHexLiteral(const char *text, size_t len);
static plObject *lexResolveStringLiteral(const char *text, size_t len);
static int lexResolveComparison(const char *text);
%}

%%

source													{return SOURCE;}
pipe													{return PIPE;}
sink													{return SINK;}
filter													{return FILTER;}
predicate												{return PREDICATE;}
struct													{return STRUCT;}
prod													{return PROD;}
drop													{return DROP;}
end														{return END;}
while													{return WHILE;}
not														{return NOT;}
and														{return AND;}
or														{return OR;}
true|false|blank|null									{yylval.object=lexResolveSimpleLiteral(yytext[0]); return LITERAL;}
if														{return IF;}
eif														{return EIF;}
else													{return ELSE;}
Any														{yylval.marker=PL_MARKER_TYPE_ANY; return TYPE;}
Int														{yylval.marker=PL_MARKER_TYPE_INT; return TYPE;}
Float													{yylval.marker=PL_MARKER_TYPE_FLOAT; return TYPE;}
Bool													{yylval.marker=PL_MARKER_TYPE_BOOL; return TYPE;}
Bytes													{yylval.marker=PL_MARKER_TYPE_BYTES; return TYPE;}
Array													{yylval.marker=PL_MARKER_TYPE_ARRAY; return TYPE;}
cont													{return CONT;}
break													{return BREAK;}
verify													{return VERIFY;}
abort													{return ABORT;}
local													{return LOCAL;}
is														{return IS;}
as														{return AS;}
import													{return IMPORT;}
export													{return EXPORT;}
main													{return MAIN;}
\?STORE/[^_A-Za-z0-9]									{yylval.marker=PL_MARKER_CONTEXT_STORE; return CONTEXT;}
"->"													{return ARROW;}
[-+*/%^&|]/[^=]											{return yytext[0];}
"<<"/[^=]												{return LSHIFT;}
">>"/[^=]												{return RSHIFT;}
[-+*/%^&|]=/[^=]										{yylval.marker=yytext[0]; return OPERATOR_ASSIGNMENT;}
"<<="													{yylval.marker=PL_MARKER_LSHIFT; return OPERATOR_ASSIGNMENT;}
">>="													{yylval.marker=PL_MARKER_RSHIFT; return OPERATOR_ASSIGNMENT;}
[=,:.(){}]												{return yytext[0];}
"["|"]"													{return yytext[0];}
;+														{return ';';}
[<>]=?|[!=]=											{yylval.marker=lexResolveComparison(yytext); return COMPARISON;}
[_A-Za-z][_A-Za-z0-9]*									{yylval.node=createOneSplitNode(NAME,registerName(yytext)); return NAME;}
([0-9]*\.?[0-9]+|[0-9]+\.)/[^.0-9A-Za-z_]				{yylval.object=lexResolveIntLiteral(yytext,yylen); return LITERAL;}
0x[A-Fa-f0-9]+											{yylval.object=lexResolveHexLiteral(yytext+2,yylen-2); return LITERAL;}
\"[^\"]*\"												{yylval.object=lexResolveStringLiteral(yytext,yylen); return LITERAL;}
[ \t]+													{}
"//".*													{}
"/*"([^*]|[\r\n]|\*[^/]?)*"*/"							{}
[\r\n]+													{}
.														{yyerror("Line %i: Invalid input: %s\n", yylineno, yytext);}
<<EOF>>													{return EOF;}

%%

static plObject *lexResolveSimpleLiteral(char firstChar) {
	switch ( firstChar ) {
		case 't':
		return &trueObject;

		case 'f':
		return &falseObject;

		case 'b':
		return &blankObject;

		default:
		return &nullObject;
	}	
}

static plObject *lexResolveIntLiteral(const char *text, size_t len) {
	plInteger *integer;

	if ( len > PL_INTEGER_MAX_DIGITS ) {
		ERROR_QUIT("Line %i: Too many (%zu) digits for an integer", yylineno, len);
	}

	integer=newInteger();
	if ( !integer ) {
		ERROR_QUIT("Failed to allocate %zu bytes", sizeof(plInteger));
	}

	for (size_t k=0; k<len; k++) {
		int ret;
		uint32_t value;
		char buffer[2];

		buffer[0]=text[len-k-1];
		buffer[1]='\0';

		value=atoi(buffer);
	}
}

static plObject *lexResoleHexLiteral(const char *text, size_t len) {
	plInteger *integer;
	unsigned int numExtraBlocks;
	ssize_t i;
	char buffer[5];

	integer=newInteger();
	if ( !integer ) {
		ERROR_QUIT("Line %i: Failed to allocate %zu bytes", yylineno, sizeof(plInteger));
	}

	if ( len > 4 ) {
		numExtraBlocks=((len-4)+3)/4;
		if ( numExtraBlocks > 255 ) {
			free(integer);
			ERROR_QUIT("Line %i: Hex string is too long (%zu characters)", yylineno, len);
		}
		integer->numExtraBlocks=numExtraBlocks;
		integer->blocks=safeMalloc(sizeof(uint32_t)*numExtraBlocks);
	}

	i=MAX((ssize_t)len-4,0);
	memcpy(buffer,text+i,len-i);
	buffer[len-i]='\0';
	integer->firstBlock=strtol(text,NULL,16);

	for (unsigned int k=0; k<numExtraBlocks; k++) {
		ssize_t j;

		j=MAX(i-4,0);
		memcpy(buffer,text+j,i-j);
		buffer[i-j]='\0';
		integer->blocks[k]=strtol(buffer,NULL,16);
		i=j;
	}

	return (plObject*)integer;
}

static plObject *lexResolveStringLiteral(const char *text, size_t len) {
	plByteArray *array;

	array=malloc(sizeof(plByteArray));
	if ( !array ) {
		ERROR_QUIT("Line %i: Failed to allocate %zu bytes", yylineno, sizeof(plByteArray));
	}

	array->length=array->capacity=len;
	arrray->bytes=malloc(len);
	if ( !array->bytes ) {
		free(array);
		ERROR_QUIT("Line %i: Failed to allocate %zu bytes", yylineno, len);
	}

	memcpy(array->bytes,text,len);
	array->flags=PL_OBJ_TYPE_BYTE_ARRAY|PL_OBJ_FLAG_TRUTHY;

	return (plObject*)array;
}

static plMarker_t lexResolveComparison(const char *text) {
	switch ( text[0] ) {
		case '=':
		return PL_MARKER_COMPARISON_EQ;

		case '!':
		return PL_MARKER_COMPARISON_NOT_EQ;

		case '<':
		return ( text[1] == '=' )? PL_MARKER_COMPARISON_LESS_EQ : PL_MARKER_COMPARISON_LESS;

		default:
		return ( text[1] == '=' )? PL_MARKER_COMPARISON_GREATER_EQ : PL_MARKER_COMPARISON_GREATER;
	}
}