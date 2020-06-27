#include <stdlib.h>

#include "ast.h"
#include "plUtil.h"
#include "plObject.h"
#include "parser.tab.h"

astNodePtr createZeroSplitNode(int lineno, int nodeType) {
	astZeroSplitNode *node;

	node=malloc(sizeof(astZeroSplitNode));
	if ( !node ) {
		ERROR_QUIT("Failed to allocate %zu bytes", sizeof(astZeroSplitNode));
	}
	node->lineno=lineno;
	node->nodeType=nodeType;

	return (astNodePtr)node;
}

astNodePtr createOneSplitNode(int lineno, int nodeType, void *first) {
	astOneSplitNode *node;

	node=malloc(sizeof(astOneSplitNode));
	if ( !node ) {
		ERROR_QUIT("Failed to allocate %zu bytes", sizeof(astOneSplitNode));
	}
	node->lineno=lineno;
	node->nodeType=nodeType;
	node->first=first;

	return (astNodePtr)node;
}

astNodePtr createTwoSplitNode(int lineno, int nodeType, void *first, void *second) {
	astTwoSplitNode *node;

	node=malloc(sizeof(astTwoSplitNode));
	if ( !node ) {
		ERROR_QUIT("Failed to allocate %zu bytes", sizeof(astTwoSplitNode));
	}
	node->lineno=lineno;
	node->nodeType=nodeType;
	node->first=first;
	node->second=second;

	return (astNodePtr)node;
}

astNodePtr createThreeSplitNode(int lineno, int nodeType, void *first, void *second, void *third) {
	astThreeSplitNode *node;

	node=malloc(sizeof(astThreeSplitNode));
	if ( !node ) {
		ERROR_QUIT("Failed to allocate %zu bytes", sizeof(astThreeSplitNode));
	}
	node->lineno=lineno;
	node->nodeType=nodeType;
	node->first=first;
	node->second=second;
	node->third=third;

	return (astNodePtr)node;
}

astNodePtr createFourSplitNode(int lineno, int nodeType, void *first, void *second, void *third, void *fourth) {
	astFourSplitNode *node;

	node=malloc(sizeof(astFourSplitNode));
	if ( !node ) {
		ERROR_QUIT("Failed to allocate %zu bytes", sizeof(astFourSplitNode));
	}
	node->lineno=lineno;
	node->nodeType=nodeType;
	node->first=first;
	node->second=second;
	node->third=third;
	node->fourth=fourth;

	return (astNodePtr)node;
}

void freeAstTree(astNodePtr root) {
	if ( root->nodeType == LITERAL ) {
		freeObject((plObject*)root->first);
		return;
	}

	switch ( nodeSplitSize(root->nodeType) ) {
		case 4:
		freeAstTree(root->fourth);
		// Intentional fall-through

		case 3:
		freeAstTree(root->third);
		// Intentional fall-through

		case 2:
		freeAstTree(root->second);
		// Intentional fall-through

		case 1:
		freeAstTree(root->first);
		break;

		default:
		break;
	}

	free(root);
}

int nodeSplitSize(int nodeType) {
	switch ( nodeType ) {
		case LITERAL:
		case NAME:
		return -1;

		case TYPE:
		case DROP:
		case END:
		case CONT:
		case BREAK:
		case CONTEXT:
		return 0;

		case PROD:
		case ABORT:
		case VERIFY:
		case MAIN:
		case IMPORT:
		case EXPORT:
		case NOT:
		case 'L':
		return 1;

		default:
		return 2;

		case SINK:
		case FILTER:
		case EIF:
		case 'A':
		return 3;

		case SOURCE:
		case PIPE:
		case PREDICATE:
		case IF:
		return 4;
	}
}
