#ifndef __PIPELINE_AST_H__
#define __PIPELINE_AST_H__

#define AST_NODE_HAS_PARENT

#include <stdio.h>

/*
Non-obvious nodetypes:

C: FUNCTION_CALL
L: ARRAY_LITERAL
*/

typedef struct astFourSplitNode *astNodePtr;

#ifdef AST_NODE_HAS_PARENT
#define AST_PARENT_DECL astNodePtr parent;
#else
#define AST_PARENT_DECL
#endif

#define AST_NODE_HEADER \
    int nodeType; \
    int marker; \
    int firstLine; \
    int firstColumn; \
    AST_PARENT_DECL

struct astFourSplitNode {
    AST_NODE_HEADER
    astNodePtr first;
    astNodePtr second;
    astNodePtr third;
    astNodePtr fourth;
};

typedef struct YYLTYPE YYLTYPE;

int formAstFromFile(FILE *infile, astNodePtr *programTree);
astNodePtr createNode(const YYLTYPE *locPtr, int nodeType, ...);
void freeAstTree(astNodePtr root);
int nodeSplitSize(int nodeType) __attribute__ ((pure));

#endif // __PIPELINE_AST_H__
