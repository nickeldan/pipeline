#ifndef __PIPELINE_PARSER_UTIL_H__
#define __PIPELINE_PARSER_UTIL_H__

#include "ast.h"

extern int yylineno;

#define CR_0_NODE(nodeType) createZeroSplitNode(yylineno,nodeType)
#define CR_1_NODE(nodeType,first) createOneSplitNode(yylineno,nodeType,first)
#define CR_2_NODE(nodeType,first,second) createTwoSplitNode(yylineno,nodeType,first,second)
#define CR_3_NODE(nodeType,first,second,third) createThreeSplitNode(yylineno,nodeType,first,second,third)
#define CR_4_NODE(nodeType,first,second,third,fourth) createFourSplitNode(yylineno,nodeType,first,second,third,fourth)

#endif // __PIPELINE_PARSER_UTIL_H__