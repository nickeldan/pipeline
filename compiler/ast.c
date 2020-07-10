#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>

#include "ast.h"
#include "plUtil.h"
#include "plObject.h"
#include "parserWrapper.h"
#include "lex.yy.h"

struct astZeroSplitNode {
	AST_NODE_HEADER
};

struct astOneSplitNode {
	AST_NODE_HEADER
	astNodePtr first;
};

struct astTwoSplitNode {
	AST_NODE_HEADER
	astNodePtr first;
	astNodePtr second;
};

struct astThreeSplitNode {
	AST_NODE_HEADER
	astNodePtr first;
	astNodePtr second;
	astNodePtr third;
};

int formAstFromFile(FILE *infile, astNodePtr *programTree) {
	int ret;
	yyscan_t scaninfo;

	yylex_init(&scaninfo);
	yyset_in(infile,scaninfo);

	ret=yyparse(programTree,scaninfo);
	yylex_destroy(scaninfo);
	return ret;
}

astNodePtr createNode(const YYLTYPE *locPtr, int nodeType, ...) {
	astNodePtr node;
	int splitSize;
	size_t size;
	va_list args;

	splitSize=nodeSplitSize(nodeType);

	switch ( splitSize ) {
		default:
		size=sizeof(struct astZeroSplitNode);
		break;

		case -1:
		case 1:
		size=sizeof(struct astOneSplitNode);
		break;

		case 2:
		size=sizeof(struct astTwoSplitNode);
		break;

		case 3:
		size=sizeof(struct astThreeSplitNode);
		break;

		case 4:
		size=sizeof(struct astFourSplitNode);
		break;
	}

	node=malloc(size);
	if ( !node ) {
		ERROR_QUIT("Failed to allocate %zu bytes", size);
	}

	memcpy(&node->location,locPtr,sizeof(YYLTYPE));
	node->nodeType=nodeType;
	node->parent=NULL;

	if ( splitSize == 0 ) {
		return node;
	}

	va_start(args,nodeType);
	if ( splitSize == -1 ) {
		node->first=va_arg(args,astNodePtr);
	}
	else {
		astNodePtr *branch;

		branch=&node->first;
		for (int k=0; k<splitSize; k++) {
			*branch=va_arg(args,astNodePtr);
			if ( *branch ) {
				(*branch)->parent=node;
			}
			branch++;
		}
	}
	va_end(args);

	return node;
}

void freeAstTree(astNodePtr root) {
	if ( !root ) {
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
		if ( root->nodeType == LITERAL ) {
			freeObject((plObject*)root->first);
		}
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
		case '_':
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
		case EIF:
		return 3;

		case SOURCE:
		case PIPE:
		case IF:
		return 4;
	}
}
