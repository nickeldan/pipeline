#ifndef __PIPELINE_AST_H__
#define __PIPELINE_AST_H__

#include "marker.h"

#define AST_NODE_HEADER \
	int nodeType; \
	int marker; \
	int lineno;

/*
	Non-obvious nodetypes:

	F: FILE
	A: ARG_FORMAT
	C: FUNCTION_CALL
	G: GLOBAL
	L: ARRAY_LITERAL
*/

struct astFourSplitNode;

typedef struct astFourSplitNode *astNodePtr;

typedef struct astZeroSplitNode {
	AST_NODE_HEADER
} astZeroSplitNode;

typedef struct astOneSplitNode {
	AST_NODE_HEADER
	astNodePtr first;
} astOneSplitNode;

typedef struct astTwoSplitNode {
	AST_NODE_HEADER
	astNodePtr first, second;
} astTwoSplitNode;

typedef struct astThreeSplitNode {
	AST_NODE_HEADER
	astNodePtr first, second, third;
} astThreeSplitNode;

typedef struct astFourSplitNode {
	AST_NODE_HEADER
	astNodePtr first, second, third, fourth;
} astFourSplitNode;

astNodePtr createZeroSplitNode(int lineno, int nodeType);
astNodePtr createOneSplitNode(int lineno, int nodeType, void *first);
astNodePtr createTwoSplitNode(int lineno, int nodeType, void *first, void *second);
astNodePtr createThreeSplitNode(int lineno, int nodeType, void *first, void *second, void *third);
astNodePtr createFourSplitNode(int lineno, int nodeType, void *first, void *second, void *third, void *fourth);
void freeAstTree(astNodePtr root);
int nodeSplitSize(int nodeType) __attribute__ ((pure));

#endif // __PIPELINE_AST_H__