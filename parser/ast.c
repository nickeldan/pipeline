#include <stdlib.h>

#include "ast.h"
#include "util.h"
#include "parser.tab.h"

astNodePtr createZeroSplitNode(int nodeType) {
	astZeroSplitNode *node;

	node=malloc(sizeof(astZeroSplitNode));
	if ( !node ) {
		ERROR_QUIT("Failed to allocate %zu bytes", sizeof(astZeroSplitNode));
	}
	node->nodeType=nodeType;

	return node;
}

astNodePtr createOneSplitNode(int nodeType, void *first) {
	astOneSplitNode *node;

	node=malloc(sizeof(astOneSplitNode));
	if ( !node ) {
		ERROR_QUIT("Failed to allocate %zu bytes", sizeof(astOneSplitNode));
	}
	node->nodeType=nodeType;
	node->first=first;

	return (astNodePtr)node;
}

astNodePtr createTwoSplitNode(int nodeType, void *first, *second) {
	astTwoSplitNode *node;

	node=malloc(sizeof(astTwoSplitNode));
	if ( !node ) {
		ERROR_QUIT("Failed to allocate %zu bytes", sizeof(astTwoSplitNode));
	}
	node->nodeType=nodeType;
	node->first=first;
	node->second=second;

	return node;
}

astNodePtr createThreeSplitNode(int nodeType, void *first, void *second, void *third) {
	astThreeSplitNode *node;

	node=malloc(sizeof(astThreeSplitNode));
	if ( !node ) {
		ERROR_QUIT("Failed to allocate %zu bytes", sizeof(astThreeSplitNode));
	}
	node->nodeType=nodeType;
	node->first=first;
	node->second=second;
	node->third=third;

	return node;
}

astNodePtr createFourSplitNode(int nodetype, void *first, void *second, void *third, void *fourth) {
	astFourSplitNode *node;

	node=malloc(sizeof(astFourSplitNode));
	if ( !node ) {
		ERROR_QUIT("Failed to allocate %zu bytes", sizeof(astFourSplitNode));
	}
	node->nodeType=nodeType;
	node->first=first;
	node->second=second;
	node->third=third;
	node->fourth=fourth;

	return node;
}

void freeAstTree(astNodePtr root) {
	astAllNodePtr all;

	if ( !root ) {
		return;
	}

	all=(astAllNodePtr)root;

	switch ( root->nodeType ) {
		case LITERAL:
		freeObject((plObject*)all->first);
		break;

		case NAME:
		case TYPE:
		case DROP:
		case END:
		case CONT:
		case BREAK:
		case CONTEXT:
		break;

		case SOURCE:
		case PIPE:
		case PREDICATE:
		case IF:
		freeTree(all->fourth);
		// Intentional fall-through.

		case SINK:
		case FILTER:
		case EIF:
		case ARG_FORMAT:
		freeTree(all->third);
		// Intentional fall-through.

		default:
		freeTree(all->second);
		// Intentional fall-through.

		case PROD:
		case ABORT:
		case VERIFY:
		case MAIN:
		case IMPORT:
		case EXPORT:
		case ARRAY_LITERAL:
		case NOT:
		freeTree(all->first);
		break;
	}

	done:

	free(root);
}