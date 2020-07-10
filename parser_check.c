#include <stdio.h>

#include "ast.h"
#include "plObject.h"
#include "parserWrapper.h"

void printTree(const astNodePtr tree, unsigned int margin);
void printMargin(unsigned int margin);
const char *nodeTypeName(int nodeType);

extern astNodePtr programTree;

int main() {
	int ret;
	astNodePtr programTree;

	ret=formAstFromFile(stdin,&programTree);
	if ( ret == 0 ) {
		printTree(programTree,0);
		freeAstTree(programTree);
	}

	return ret;
}

void printTree(const astNodePtr tree, unsigned int margin) {
	int splitSize;

	if ( !tree ) {
		return;
	}

	printMargin(margin);
	printf("%s", nodeTypeName(tree->nodeType));
	if ( tree->nodeType == NAME ) {
		printf(" (%s)", (const char*)(tree->first));
	}
	printf("\n");

	splitSize=nodeSplitSize(tree->nodeType);

	if ( splitSize > 0 ) {
		astNodePtr *field;

		field=&tree->first;

		printMargin(margin);
		printf("{\n");

		for (int k=0; k<splitSize; k++) {
			printTree(*field,margin+1);
			field++;
		}

		printMargin(margin);
		printf("}\n");
	}
}

void printMargin(unsigned int margin) {
	for (unsigned int k=0; k<margin; k++) {
		printf("\t");
	}
}

const char *nodeTypeName(int nodeType) {
	static char string[2];

	switch ( nodeType ) {
		case SOURCE: return "SOURCE";
		case PIPE: return "PIPE";
		case SINK: return "SINK";
		case LOCAL: return "LOCAL";
		case STRUCT: return "STRUCT";
		case WHILE: return "WHILE";
		case IF: return "IF";
		case EIF: return "EIF";
		case ELSE: return "ELSE";
		case PROD: return "PROD";
		case DROP: return "DROP";
		case END: return "END";
		case NOT: return "NOT";
		case CONT: return "CONT";
		case BREAK: return "BREAK";
		case VERIFY: return "VERIFY";
		case ABORT: return "ABORT";
		case IS: return "IS";
		case AS: return "AS";
		case IMPORT: return "IMPORT";
		case EXPORT: return "EXPORT";
		case MAIN: return "MAIN";
		case ARROW: return "ARROW";
		case LSHIFT: return "<<";
		case RSHIFT: return ">>";
		case AND: return "AND";
		case OR: return "OR";
		case NAME: return "NAME";
		case LITERAL: return "LITERAL";
		case TYPE: return "TYPE";
		case COMPARISON: return "COMPARISON";
		case OPERATOR_ASSIGNMENT: return "OPERATOR_ASSIGNMENT";
		case CONTEXT: return "CONTEXT";
		default:
			string[0]=nodeType;
			string[1]='\0';
			return string;
	}
}
