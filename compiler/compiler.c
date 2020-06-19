#include "ast.h"
#include "dll.h"
#include "module.h"
#include "plError.h"

static int recursivelyCompileTree(astNodePtr tree, dll *list);

void compileTree(astNodePtr tree) {
	int ret;
	dll list={.head=NULL, .tail=NULL};

	ret=recursivelyCompileTree(tree, &list);
}

static int recursivelyCompileTree(astNodePtr tree, dll *list) {

}
