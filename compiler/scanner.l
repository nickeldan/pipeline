%option nodefault yylineno noyywrap reentrant bison-bridge bison-locations
%option header-file="lex.yy.h"

%{
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "plObject.h"
#include "ast.h"
#include "marker.h"
#include "nameTable.h"
#include "plUtil.h"
#include "parserWrapper.h"

#define YY_USER_ACTION \
    do { \
        yylloc->first_line = yylloc->last_line; \
        yylloc->first_column = yylloc->last_column; \
        if ( yylloc->last_line == yylineno ) { \
            yylloc->last_column += yyleng; \
        } \
        else { \
            yylloc->last_line = yylineno; \
            yylloc->last_column = yytext + yyleng - strrchr(yytext, '\n'); \
        } \
    } while (0);

static plObject *lexResolveSimpleLiteral(yyscan_t yyscanner, char firstChar);
static plObject *lexResolveHexLiteral(yyscan_t yyscanner, const char *text, size_t len);
static plObject *lexResolveStringLiteral(yyscan_t yyscanner, const char *text, size_t len);
static int lexResolveContext(yyscan_t yyscanner, const char *text, size_t len);
static int lexResolveComparison(yyscan_t yyscanner, const char *text);
%}

%%

source                                                    {return SOURCE;}
pipe                                                    {return PIPE;}
sink                                                    {return SINK;}
struct                                                    {return STRUCT;}
prod                                                    {return PROD;}
drop                                                    {return DROP;}
end                                                        {return END;}
while                                                    {return WHILE;}
not                                                        {return NOT;}
and                                                        {return AND;}
or                                                        {return OR;}
true|false|blank|null                                    {yylval->object=lexResolveSimpleLiteral(yyscanner,yytext[0]); return LITERAL;}
if                                                        {return IF;}
eif                                                        {return EIF;}
else                                                    {return ELSE;}
Any                                                        {yylval->marker=PL_MARKER_TYPE_ANY; return TYPE;}
Int                                                        {yylval->marker=PL_MARKER_TYPE_INT; return TYPE;}
Float                                                    {yylval->marker=PL_MARKER_TYPE_FLOAT; return TYPE;}
Bool                                                    {yylval->marker=PL_MARKER_TYPE_BOOL; return TYPE;}
Bytes                                                    {yylval->marker=PL_MARKER_TYPE_BYTES; return TYPE;}
Array                                                    {yylval->marker=PL_MARKER_TYPE_ARRAY; return TYPE;}
cont                                                    {return CONT;}
break                                                    {return BREAK;}
verify                                                    {return VERIFY;}
abort                                                    {return ABORT;}
local                                                    {return LOCAL;}
is                                                        {return IS;}
as                                                        {return AS;}
import                                                    {return IMPORT;}
export                                                    {return EXPORT;}
main                                                    {return MAIN;}
\?[A-Z]+/[^_a-z0-9]                                        {yylval->marker=lexResolveContext(yyscanner,yytext+1,yyleng-1); return CONTEXT;}
"->"                                                    {return ARROW;}
[-+*/%^&|_]/[^=]                                        {return yytext[0];}
"<<"/[^=]                                                {return LSHIFT;}
">>"/[^=]                                                {return RSHIFT;}
[-+*/%^&|]=/[^=]                                        {yylval->marker=yytext[0]; return OPERATOR_ASSIGNMENT;}
"<<="                                                    {yylval->marker=PL_MARKER_LSHIFT; return OPERATOR_ASSIGNMENT;}
">>="                                                    {yylval->marker=PL_MARKER_RSHIFT; return OPERATOR_ASSIGNMENT;}
[=,:.(){}]                                                {return yytext[0];}
"["|"]"                                                    {return yytext[0];}
;+                                                        {return ';';}
[<>]=?|[!=]=                                            {yylval->marker=lexResolveComparison(yyscanner,yytext); return COMPARISON;}
[_A-Za-z][_A-Za-z0-9]*                                    {yylval->node=createNode(yylloc,NAME,registerName(yytext)); return NAME;}
-?([0-9]*\.?[0-9]+|[0-9]+\.)/[^.0-9A-Za-z_]                {    int localErrnum=plIntegerFromString((plInteger**)&yylval->object,yytext,yyleng);
                                                            switch ( localErrnum )
                                                            {
                                                                case PL_ERROR_OK:
                                                                return LITERAL;

                                                                case PL_ERROR_OUT_OF_MEMORY:
                                                                ERROR_QUIT("Memory allocation error on line %i", yylineno);

                                                                case PL_ERROR_BAD_INPUT:
                                                                ERROR_QUIT("Lexical error on line %i: Invalid integer literal: %s", yylineno, yytext);

                                                                default:
                                                                ERROR_QUIT("Unknown error (%i) on line %i", localErrnum, yylineno);
                                                            }
                                                        }
-?0x[A-Fa-f0-9]+/[^_G-Zg-z]                                {yylval->object=lexResolveHexLiteral(yyscanner,yytext,yyleng); return LITERAL;}
\"[^\"]*\"                                                {yylval->object=lexResolveStringLiteral(yyscanner,yytext,yyleng); return LITERAL;}
[ \t]+                                                    {}
"//".*                                                    {}
"/*"([^*]|[\r\n]|\*[^/]?)*"*/"                            {}
[\r\n]+                                                    {}
.                                                        {ERROR_QUIT("Lexical error on line %i: Invalid input: %s\n", yylineno, yytext);}

%%

static plObject *lexResolveSimpleLiteral(yyscan_t yyscanner, char firstChar) {
    SETUP_SCANNER;

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

static plObject *lexResolveHexLiteral(yyscan_t yyscanner, const char *text, size_t len) {
    SETUP_SCANNER;
    plInteger *integer;
    unsigned int numExtraBlocks;
    ssize_t i;
    char buffer[5];

    integer=plIntegerNew();
    if ( !integer ) {
        ERROR_QUIT("Memory allocation error on line %i: Failed to allocate %zu bytes", yylineno, sizeof(plInteger));
    }

    if ( text[0] == '-' ) {
        integer->flags|=PL_OBJ_FLAG_NEGATIVE;
        text+=3;
        len-=3;
    }
    else {
        text+=2;
        len-=2;
    }

    if ( len > 4 ) {
        numExtraBlocks=((len-4)+3)/4;
        if ( numExtraBlocks > 255 ) {
            free(integer);
            ERROR_QUIT("Lexical error on line %i: Hex string is too long (%zu characters)", yylineno, len);
        }
        integer->numExtraBlocks=numExtraBlocks;
        integer->blocks=SAFE_MALLOC(sizeof(uint32_t)*numExtraBlocks);
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

static plObject *lexResolveStringLiteral(yyscan_t yyscanner, const char *text, size_t len) {
    SETUP_SCANNER;
    plByteArray *array;

    array=malloc(sizeof(plByteArray));
    if ( !array ) {
        ERROR_QUIT("Line %i: Failed to allocate %zu bytes", yylineno, sizeof(plByteArray));
    }

    array->length=array->capacity=len;
    array->bytes=malloc(len);
    if ( !array->bytes ) {
        free(array);
        ERROR_QUIT("Line %i: Failed to allocate %zu bytes", yylineno, len);
    }

    memcpy(array->bytes,text,len);
    array->flags=PL_OBJ_TYPE_BYTE_ARRAY|PL_OBJ_FLAG_TRUTHY;

    return (plObject*)array;
}

static int lexResolveContext(yyscan_t yyscanner, const char *text, size_t len) {
    SETUP_SCANNER;

    if ( strncmp(text,"STORE",len) == 0 ) {
        return PL_MARKER_CONTEXT_STORE;
    }
    else if ( strncmp(text,"FILE",len) == 0 ) {
        return PL_MARKER_CONTEXT_FILE;
    }
    else if ( strncmp(text,"LINE",len) == 0 ) {
        return PL_MARKER_CONTEXT_LINE;
    }
    else {
        ERROR_QUIT("Lexical error on line %i: Invalid context identifier: %s", yylineno, text);
    }
}

static int lexResolveComparison(yyscan_t yyscanner, const char *text) {
    SETUP_SCANNER;

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
