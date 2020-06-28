#ifndef __PIPELINE_PARSER_UTIL_H__
#define __PIPELINE_PARSER_UTIL_H__

#include <stdarg.h>

#include "ast.h"

extern int yylineno;

#define NODE(...) createNode(yylineno, __VA_ARGS__)

#endif // __PIPELINE_PARSER_UTIL_H__
