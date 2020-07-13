#ifndef __PIPELINE_AST_H__
#define __PIPELINE_AST_H__

#define AST_NODE_HAS_PARENT

#include <stdio.h>

// copied from parser.tab.h
#if !defined(YYLTYPE) && !defined(YYLTYPE_IS_DECLARED)
#define YYLTYPE_IS_DECLARED 1
#define YYLTYPE_IS_TRIVIAL 1

typedef struct YYLTYPE {
    int first_line;
    int first_column;
    int last_line;
    int last_column;
} YYLTYPE;
#endif

/*
Non-obvious nodetypes:

C: FUNCTION_CALL
L: ARRAY_LITERAL
*/

#ifdef AST_NODE_HAS_PARENT
#define AST_PARENT_DECL struct astFourSplitNode *parent;
#endif

#define AST_NODE_HEADER \
    YYLTYPE location;\
    int nodeType; \
    int marker; \
    AST_PARENT_DECL

typedef struct astFourSplitNode {
    AST_NODE_HEADER
    struct astFourSplitNode *first;
    struct astFourSplitNode *second;
    struct astFourSplitNode *third;
    struct astFourSplitNode *fourth;
} *astNodePtr;

int formAstFromFile(FILE *infile, astNodePtr *programTree);
astNodePtr createNode(const YYLTYPE *locPtr, int nodeType, ...);
void freeAstTree(astNodePtr root);
int nodeSplitSize(int nodeType) __attribute__ ((pure));

#endif // __PIPELINE_AST_H__
