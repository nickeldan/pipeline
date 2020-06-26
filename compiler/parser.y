%{
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "parserUtil.h"
#include "marker.h"
#include "nameTable.h"
#include "plObject.h"
#include "compiler.h"

void yyerror(const char *message, ...);
int yylex(void);

static astNodePtr resolveAttributes(astNodePtr object, astNodePtr attributes);
%}

%union {
	astNodePtr node;
	plObject *object;
	int marker;
}

%token SOURCE PIPE SINK FILTER
%token PREDICATE LOCAL STRUCT WHILE IF EIF ELSE PROD DROP END NOT
%token CONT BREAK VERIFY ABORT IS AS IMPORT EXPORT MAIN
%token ARROW LSHIFT RSHIFT AND OR
%token <node> NAME
%token <object> LITERAL
%token <marker> TYPE COMPARISON OPERATOR_ASSIGNMENT CONTEXT

%type <node> file file_section global_content main_definition import_statement export_statement
%type <node> struct_definition global_var_definition struct_field type attribute_trail moduled_name
%type <node> source_definition pipe_definition sink_definition filter_definition local_definition
%type <node> predicate_definition optional_name arg_list arg_element possible_assignment
%type <node> compilation_expression compilation_array_literal compilation_array_element
%type <node> expression array_literal array_element
%type <node> statement_list actual_statement_list general_statement if_section eif_section else_section while_section
%type <node> statement function_call call_list call_arg arrow_statement arrow_sender
%type <node> arrow_sender_item arrow_sender_list arrow_receiver arrow_receiver_item

%left AND OR
%left NOT
%nonassoc COMPARISON
%left ','
%left '+' '-'
%left '*' '/'
%left '%'
%left '^' '|' '&'
%left LSHIFT
%left RSHIFT
%nonassoc '['
%left '.'

%%

file: file_section {compileTree($1);}
	;

file_section: global_content {$$=$1;}
	| file_section global_content {$$ = CR_2_NODE('F',$1,$2);}
	;

global_content: main_definition {$$=$1;}
	| import_statement {$$=$1;}
	| export_statement {$$=$1;}
	| struct_definition {$$=$1;}
	| source_definition {$$=$1;}
	| pipe_definition {$$=$1;}
	| sink_definition {$$=$1;}
	| filter_definition {$$=$1;}
	| predicate_definition {$$=$1;}
	| global_var_definition {$$=$1;}
	;

main_definition: MAIN '{' statement_list '}' {$$=CR_1_NODE(MAIN,$3);}
	;

import_statement: IMPORT NAME ';' {$$=CR_1_NODE(IMPORT,$2);}
	;

export_statement: EXPORT NAME ';' {$$=CR_1_NODE(EXPORT,$2);}
	;

struct_definition: STRUCT NAME '{' struct_field '}' {$$=CR_2_NODE(STRUCT,$2,$4);}
	;

struct_field: NAME ':' type {$$=CR_2_NODE(':',$1,$3);}
	| struct_field ',' NAME ':' type {$$=CR_2_NODE(',',$1,CR_2_NODE(':',$3,$5));}
	;

type: TYPE {$$=CR_0_NODE(TYPE); $$->marker=$1;}
	| moduled_name {$$=$1;}
	;

moduled_name: NAME attribute_trail {$$=resolveAttributes($1,$2);}
	;

attribute_trail: {$$=NULL;}
	| attribute_trail '.' NAME {$$=$1? CR_2_NODE('.',$1,$3) : $3;}
	;

source_definition: SOURCE optional_name '(' arg_list ')' ARROW type '{' statement_list '}' {$$=CR_4_NODE(SOURCE,$2,$4,$7,$9);}
	;

pipe_definition: PIPE optional_name '(' arg_list ')' ARROW type '{' statement_list '}' {$$=CR_4_NODE(PIPE,$2,$4,$7,$9);}
	;

sink_definition: SINK optional_name '(' arg_list ')' '{' statement_list '}' {$$=CR_3_NODE(SINK,$2,$4,$7);}
	;

local_definition: LOCAL '(' arg_list ')' '{' statement_list '}' {$$=CR_2_NODE(LOCAL,$3,$6);}
	;

filter_definition: FILTER optional_name '(' arg_list ')' '{' statement_list '}' {$$=CR_3_NODE(FILTER,$2,$4,$7);}
	;

predicate_definition: PREDICATE NAME '(' NAME ':' type ')' '{' statement_list '}' {$$=CR_4_NODE(PREDICATE,$2,$4,$6,$9);}
	;

global_var_definition: compilation_expression ARROW NAME ';' {$$=CR_2_NODE('G',$1,$3);}
	;

optional_name: {$$=NULL;}
	| NAME {$$=$1;}
	;

arg_list: {$$=NULL;}
	| arg_element {$$=$1;}
	;

arg_element: NAME ':' type possible_assignment {$$=CR_3_NODE('A',$1,$3,$4);}
	| arg_element ',' NAME ':' type possible_assignment {$$=CR_2_NODE(',',$1,CR_3_NODE('A',$3,$5,$6));}
	;

possible_assignment: {$$=NULL;}
	| '=' compilation_expression {$$=$2;}
	;

compilation_expression: moduled_name {$$=$1;}
	| LITERAL attribute_trail {$$=resolveAttributes(CR_1_NODE(LITERAL,$1),$2);}
	| compilation_expression '[' compilation_expression ']' attribute_trail {$$=resolveAttributes(CR_2_NODE('[',$1,$3),$5);}
	| '[' compilation_array_literal ']' attribute_trail {$$=resolveAttributes(CR_1_NODE('L',$2),$4);}
	| '(' compilation_expression ')' attribute_trail {$$=resolveAttributes($2,$4);}
	| function_call attribute_trail {$$=resolveAttributes($1,$2);}
	| compilation_expression AND compilation_expression {$$=CR_2_NODE(AND,$1,$3);}
	| compilation_expression OR compilation_expression {$$=CR_2_NODE(OR,$1,$3);}
	| NOT compilation_expression {$$=CR_1_NODE(NOT,$2);}
	| compilation_expression '+' compilation_expression {$$=CR_2_NODE('+',$1,$3);}
	| compilation_expression '-' compilation_expression {$$=CR_2_NODE('-',$1,$3);}
	| compilation_expression '*' compilation_expression {$$=CR_2_NODE('*',$1,$3);}
	| compilation_expression '/' compilation_expression {$$=CR_2_NODE('/',$1,$3);}
	| compilation_expression '%' compilation_expression {$$=CR_2_NODE('%',$1,$3);}
	| compilation_expression '^' compilation_expression {$$=CR_2_NODE('^',$1,$3);}
	| compilation_expression '|' compilation_expression {$$=CR_2_NODE('|',$1,$3);}
	| compilation_expression '&' compilation_expression {$$=CR_2_NODE('&',$1,$3);}
	| compilation_expression LSHIFT compilation_expression {$$=CR_2_NODE(LSHIFT,$1,$3);}
	| compilation_expression RSHIFT compilation_expression {$$=CR_2_NODE(RSHIFT,$1,$3);}
	;

expression: moduled_name {$$=$1;}
	| LITERAL attribute_trail {$$=resolveAttributes(CR_1_NODE(LITERAL,$1),$2);}
	| CONTEXT attribute_trail {$$=CR_0_NODE(CONTEXT); $$->marker=$1; if ( $2 ) {$$=resolveAttributes($$,$2);}}
	| expression '[' expression ']' attribute_trail {$$=resolveAttributes(CR_2_NODE('[',$1,$3),$5);}
	| '[' array_literal ']' attribute_trail {$$=resolveAttributes(CR_1_NODE('L',$2),$4);}
	| '(' expression ')' attribute_trail {$$=resolveAttributes($2,$4);}
	| function_call attribute_trail {$$=resolveAttributes($1,$2);}
	| expression AND expression {$$=CR_2_NODE(AND,$1,$3);}
	| expression OR expression {$$=CR_2_NODE(OR,$1,$3);}
	| NOT expression {$$=CR_1_NODE(NOT,$2);}
	| expression COMPARISON expression {$$=CR_2_NODE(COMPARISON,$1,$3); $$->marker=$2;}
	| expression '+' expression {$$=CR_2_NODE('+',$1,$3);}
	| expression '-' expression {$$=CR_2_NODE('-',$1,$3);}
	| expression '*' expression {$$=CR_2_NODE('*',$1,$3);}
	| expression '/' expression {$$=CR_2_NODE('/',$1,$3);}
	| expression '%' expression {$$=CR_2_NODE('%',$1,$3);}
	| expression '^' expression {$$=CR_2_NODE('^',$1,$3);}
	| expression '|' expression {$$=CR_2_NODE('|',$1,$3);}
	| expression '&' expression {$$=CR_2_NODE('&',$1,$3);}
	| expression LSHIFT expression {$$=CR_2_NODE(LSHIFT,$1,$3);}
	| expression RSHIFT expression {$$=CR_2_NODE(RSHIFT,$1,$3);}
	| moduled_name IS type {$$=CR_2_NODE(IS,$1,$3);}
	| '(' expression ')' IS type {$$=CR_2_NODE(IS,$2,$5);}
	| '(' arrow_statement ')' {$$=$2;}
	;

compilation_array_literal: {$$=NULL;}
	| compilation_array_element {$$=$1;}
	;

compilation_array_element: compilation_expression {$$=$1;}
	| compilation_array_element ',' compilation_expression {$$=CR_2_NODE(',',$1,$3);}
	;

array_literal: {$$=NULL;}
	| array_element {$$=$1;}
	;

array_element: expression {$$=$1;}
	| array_element ',' expression {$$=CR_2_NODE(',',$1,$3);}
	;

function_call: moduled_name '(' call_list ')' {$$=CR_2_NODE('C',$1,$3);}
	;

call_list: {$$=NULL;}
	| call_arg {$$=$1;}
	| call_list ',' call_arg {$$=CR_2_NODE(',',$1,$3);}
	;

call_arg: expression {$$=$1;}
	| NAME '=' expression {$$=CR_2_NODE('=',$1,$3);}
	;

statement_list: {$$=NULL;}
	| actual_statement_list {$$=$1;}
	;

actual_statement_list: general_statement {$$=$1;}
	| actual_statement_list general_statement {$$=$1? CR_2_NODE(';',$1,$2) : $2;}
	;

general_statement: ';' {$$=NULL;}
	| statement ';' {$$=$1;}
	| if_section {$$=$1;}
	| while_section {$$=$1;}
	;

if_section: IF expression '{' statement_list '}' eif_section else_section {$$=CR_4_NODE(IF,$2,$4,$6,$7);}
	;

eif_section: {$$=NULL;}
	| eif_section EIF expression '{' statement_list '}' {$$=CR_3_NODE(EIF,$1,$3,$5);}
	;

else_section: {$$=NULL;}
	| ELSE '{' statement_list '}' {$$=$3;}
	;

while_section: WHILE expression '{' statement_list '}' {$$=CR_2_NODE(WHILE,$2,$4);}
	;

statement: PROD expression {$$=CR_1_NODE(PROD,$2);}
	| DROP {$$=CR_0_NODE(DROP);}
	| END {$$=CR_0_NODE(END);}
	| CONT {$$=CR_0_NODE(CONT);}
	| BREAK {$$=CR_0_NODE(BREAK);}
	| ABORT expression {$$=CR_1_NODE(ABORT,$2);}
	| VERIFY expression {$$=CR_1_NODE(VERIFY,$2);}
	| NAME AS type {$$=CR_2_NODE(AS,$1,$3);}
	| expression OPERATOR_ASSIGNMENT expression {$$=CR_2_NODE(OPERATOR_ASSIGNMENT,$1,$3); $$->marker=$2;}
	| arrow_statement {$$=$1;}
	;

arrow_statement: arrow_sender ARROW arrow_receiver {$$=CR_2_NODE(ARROW,$1,$3);}
	;

arrow_sender: arrow_sender_item {$$=$1;}
	| '(' arrow_sender_list ',' arrow_sender_item ')' {$$=CR_2_NODE(',',$2,$4);}
	;

arrow_sender_item: expression {$$=$1;}
	| source_definition {$$=$1;}
	;

arrow_sender_list: arrow_sender_item {$$=$1;}
	| arrow_sender_list ',' arrow_sender_item {$$=CR_2_NODE(',',$1,$3);}
	;

arrow_receiver: arrow_receiver_item {$$=$1;}
	| arrow_receiver ARROW arrow_receiver_item {$$=CR_2_NODE(ARROW,$1,$3);}
	;

arrow_receiver_item: moduled_name {$$=$1;}
	| moduled_name ':' NAME {$$=CR_2_NODE(':',$1,$3);}
	| TYPE attribute_trail {$$=CR_0_NODE(TYPE); $$->marker=$1; $$=resolveAttributes($$,$2);}
	| pipe_definition attribute_trail {$$=resolveAttributes($1,$2);}
	| sink_definition attribute_trail {$$=resolveAttributes($1,$2);}
	| sink_definition attribute_trail ':' NAME {$$=CR_2_NODE(':',resolveAttributes($1,$2),$4);}
	| local_definition attribute_trail {$$=resolveAttributes($1,$2);}
	;

%%

void yyerror(const char *format, ...) {
	va_list args;

	va_start(args,format);
	vfprintf(stderr,format,args);
	va_end(args);
}

static astNodePtr resolveAttributes(astNodePtr object, astNodePtr attributes) {
	return attributes? CR_2_NODE('.',object,attributes) : object;
}
