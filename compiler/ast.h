#ifndef __PIPELINE_AST_H__
#define __PIPELINE_AST_H__

/*
	Non-obvious nodetypes:

	F: FILE
	C: FUNCTION_CALL
	L: ARRAY_LITERAL
*/

#define AST_NODE_HEADER \
	int nodeType; \
	int marker; \
	int lineno; \
	struct astFourSplitNode *parent;

typedef struct astFourSplitNode {
	AST_NODE_HEADER
	struct astFourSplitNode *first;
	struct astFourSplitNode *second;
	struct astFourSplitNode *third;
	struct astFourSplitNode *fourth;
} *astNodePtr;

astNodePtr createNode(int lineno, int nodeType, ...);
void freeAstTree(astNodePtr root);
int nodeSplitSize(int nodeType) __attribute__ ((pure));

#endif // __PIPELINE_AST_H__
