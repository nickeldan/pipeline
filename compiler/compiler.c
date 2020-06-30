#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>

#include "ast.h"
#include "codeBlock.h"
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
	codeBlock block;
	contextStack stack;
} compilationContext;

typedef uint32_t refFlags_t;

#define VAR_REF_ANY 0x000000ff
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

#define VAR_REF_NOT_OBJECT 0x0000ff00
#define VAR_REF_MODULE 0x00000100
#define VAR_REF_SOURCE 0x00000200
#define VAR_REF_PIPE 0x00000400
#define VAR_REF_SINK 0x00000800
#define VAR_REF_FILTER 0x00001000
#define VAR_REF_PREDICATE 0x00002000
#define VAR_REF_STRUCT_TYPE 0x00004000
#define VAR_REF_CONSTANT 0x00010000

typedef struct variableReferenceType {
	refFlags_t flags;
	plModuleId_t moduleId;
	plStructId_t structId;
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

static int recursivelyCompileTree(const astNodePtr tree, compilationContext *ctx);
static void pushContextToStack(int nodeType, contextStack *stack);
static int popContextFromStack(contextStack *stack);
static int compileStatementList(const astNodePtr tree, compilationContext *ctx, codeBlock *block);
static void resolveReference(const astNodePtr node, const compilationContext *ctx, variableReference *ref);
static int moduleAddMain(plModule *module, codeBlock *block);
static int moduleAddImport(plModule *module, const char *name);
static int moduleAddExport(plModule *module, const char *name);
static int compileStructDefinition(astNodePtr tree, plModule *module);
static int compileFunction(astNodePtr tree, compilationContext *ctx);
static int resolveCompilationLiteral(const astNodePtr tree, plObject **object);
static int moduleAddGlobalLiteral(plModule *module, plObject *object, const char *name);

void compileTree(astNodePtr tree) {
	int ret;
	compilationContext ctx;

	memset(&ctx,0,sizeof(compilationContext));
	moduleInit(&ctx.module);

	ret=recursivelyCompileTree(tree,&ctx);


}

static int recursivelyCompileTree(const astNodePtr tree, compilationContext *ctx) {
	int ret, nodeType;
	plObject *object;

	if ( !tree ) {
		return PL_ERROR_OK;
	}

	switch ( tree->nodeType ) {
		case 'F':
		ret=recursivelyCompileTree(tree->first,ctx);
		tree->first=NULL;
		if ( ret != PL_ERROR_OK ) {
			goto done;
		}
		ret=recursivelyCompileTree(tree->second,ctx);
		tree->second=NULL;
		break;

		case MAIN:
		ret=pushContextToStack(MAIN,&ctx->stack);
		if ( ret != PL_ERROR_OK ) {
			goto done;
		}
		ret=compileStatementList(tree->first,ctx,&ctx->block);
		if ( ret != PL_ERROR_OK ) {
			goto done;
		}
		ret=moduleAddMain(tree->first,&ctx->block);
		freeCodeBlock(&ctx->block);
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

		case 'G':
		ret=resolveCompilationLiteral(tree->first,&object);
		if ( ret != PL_ERROR_OK ) {
			goto done;
		}
		ret=moduleAddGlobalLiteral(&ctx->module,object,(const char*)tree->second);
		if ( ret != PL_ERROR_OK ) {
			freeObject(object);
		}
		break;

	}

	done:

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

static int compileStatementList(const astNodePtr tree, compilationContext *ctx, codeBlock *block) {

}

static void resolveReference(const astNodePtr node, const compilationContext *ctx, variableReference *ref) {

}

static int moduleAddMain(plModule *module, codeBlock *block) {

}

static int moduleAddImport(plModule *module, const char *name) {

}

static int moduleAddExport(plModule *module, const char *name) {

}

static int compileStructDefinition(astNodePtr tree, plModule *module) {

}

static int compileFunction(astNodePtr tree, compilationContext *ctx) {

}

static int resolveCompilationLiteral(const astNodePtr tree, plObject **object) {

}

static int moduleAddGlobalLiteral(plModule *module, plObject *object, const char *name) {

}
