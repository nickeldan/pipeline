#include "ast.h"
#include "dll.h"
#include "plModule.h"
#include "plError.h"

static int recursivelyCompileTree(astNodePtr tree, plModule *module, dll *list);

void compileTree(astNodePtr tree) {
	int ret;
	plModule module;

	moduleInit(&module);

	ret=recursivelyCompileTree(tree,&module,NULL);
}

static int recursivelyCompileTree(astNodePtr tree, plModule *module, dll *list) {

}
