#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>

#include "ast.h"
#include "dll.h"
#include "plObject.h"
#include "plModule.h"
#include "plError.h"

typedef struct contextStack {
	int *values;
	size_t capacity;
	size_t length;
} contextStack;

#define CONTEXT_STACK_INITIAL_CAPACITY 10

typedef struct compilationContext {
	plModule module;
	dll statementList;
	contextStack stack;
} compilationContext;

typedef uint16_t refFlags_t;

#define VAR_REF_ANY 0x00ff
#define VAR_REF_NULL PL_OBJ_TYPE_NULL
#define VAR_REF_INT PL_OBJ_TYPE_INT
#define VAR_REF_FLOAT PL_OBJ_TYPE_FLOAT
#define VAR_REF_NUM PL_OBJ_PRED_NUM
#define VAR_REF_BOOL PL_OBJ_TYPE_BOOL
#define VAR_REF_GEN_ARRAY PL_OBJ_PRED_GEN_ARRAY
#define VAR_REF_TRUE_ARRAY PL_OBJ_PRED_TRUE_ARRAY
#define VAR_REF_ARRAY PL_OBJ_TYPE_ARRAY
#define VAR_REF_STRUCT PL_OBJ_TYPE_STRUCT
#define VAR_REF_BYTE_ARRAY PL_OBJ_TYPE_BYTE_ARRAY
#define VAR_REF_BLANK PL_OBJ_TYPE_BLANK

#define VAR_REF_NOT_OBJECT 0xff00
#define VAR_REF_MODULE 0x0100
#define VAR_REF_SOURCE 0x0200
#define VAR_REF_PIPE 0x0400
#define VAR_REF_SINK 0x0800
#define VAR_REF_FILTER 0x1000
#define VAR_REF_PREDICATE 0x2000
#define VAR_REF_STRUCT_DEF 0x4000
#define VAR_REF_UNRESOLVED 0x8000

typedef struct variableReferenceType {
	plModuleId_t moduleId;
	plStructId_t structId;
	refFlags_t flags;
} variableReferenceType;

typedef struct argumentSignature {
	const char *name;
	variableReferenceType type;
	uint8_t hasDefaultValue;
} argumentSignature;

typedef struct functionSignature {
	argumentSignature *args;
	variableReferenceType returnType;
	uint8_t numArgs;
} functionSignature;

typedef struct structSignature {
	argumentSignature *fields;
	uint8_t numFields;
} structSignature;

typedef struct variableReference {
	union {
		functionSignature fSig;
		structSignature sSig;
	} extra;
	variableReferenceType type;
} variableReference;

static int recursivelyCompileTree(astNodePtr tree, compilationContext *ctx);
static void pushContextToStack(int nodeType, contextStack *stack);
static int popContextFromStack(contextStack *stack);
static void resolveReference(const astNodePtr node, const compilationContext *ctx, variableReference *ref);
static int moduleAddMain(plModule *module, dll *statementList);
static int moduleAddImport(plModule *module, const char *name);
static int moduleAddExport(plModule *module, const char *name);
static int compileStructDefinition(astNodePtr tree, plModule *module);
static int compileFunction(astNodePtr tree, compilationContext *ctx);

void compileTree(astNodePtr tree) {
	int ret;
	compilationContext ctx;

	memset(&ctx,0,sizeof(compilationContext));
	moduleInit(&ctx.module);

	ret=recursivelyCompileTree(tree,&ctx);


}

static int recursivelyCompileTree(astNodePtr tree, compilationContext *ctx) {
	int ret, nodeType;

	if ( !tree ) {
		return PL_ERROR_OK;
	}

	switch ( tree->nodeType ) {
		case 'F':
		ret=recursivelyCompileTree(tree->first,ctx);
		tree->first=NULL;
		if ( ret == PL_ERROR_OK ) {
			ret=recursivelyCompileTree(tree->second,ctx);
			tree->second=NULL;
		}
		break;

		case MAIN:
		ret=recursivelyCompileTree(tree->first,ctx);
		if ( ret == PL_ERROR_OK ) {
			ret=moduleAddMain(tree->first,&ctx->statementList);
			deleteList(&ctx->statementList);
		}
		break;

		case IMPORT:
		ret=moduleAddImport(&ctx->module,(const char*)tree->first);
		break;

		case EXPORT:
		ret=moduleAddExport(&ctx->module,(const char*)tree->first);
		break;

		case STRUCT:
		ret=compileStructDefinition(tree,&ctx->module);
		break;

		case SOURCE:
		case PIPE:
		case SINK:
		case LOCAL:
		case FILTER:
		case PREDICATE:
		ret=compileFunction(tree,ctx);
		break;

		
	}

	freeAstTree(tree);
	return ret;
}

static void pushContextToStack(int nodeType, contextStack *stack) {
	if ( stack->length >= stack->capacity ) {
		size_t newCapacity;

		newCapacity=( stack->capacity == 0 )? CONTEXT_STACK_INITIAL_CAPACITY : (stack->capacity + 1)*5/4;
		stack->values=realloc(stack->values,sizeof(int)*newCapacity);
		if ( !stack->values ) {
			ERROR_QUIT("Failed to allocate %zu bytes", sizeof(int)*newCapacity);
		}
		stack->capacity=newCapacity;
	}

	stack->values[stack->length++]=nodeType;
}

static int popContextFromStack(contextStack *stack) {
	if ( stack->length == 0 ) {
		ERROR_QUIT("Tried to pop from an empty stack");
	}

	return stack->values[--stack->length];
}

static void resolveReference(const astNodePtr node, const compilationContext *ctx, variableReference *ref) {

}

static int moduleAddMain(plModule *module, dll *statementList) {

}

static int moduleAddImport(plModule *module, const char *name) {

}

static int moduleAddExport(plModule *module, const char *name) {

}

static int compileStructDefinition(astNodePtr tree, plModule *module) {

}

static int compileFunction(astNodePtr tree, compilationContext *ctx) {

}