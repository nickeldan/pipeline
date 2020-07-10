#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>

#include "ast.h"
#include "plObject.h"
#include "plModule.h"
#include "plError.h"
#include "plUtil.h"

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
#define VAR_REF_STRUCT_TYPE 0x00001000

#define VAR_REF_CONSTANT 0x01000000
#define VAR_REF_UNRESOLVED 0x02000000

typedef struct variableReferenceType {
	refFlags_t flags;
	plStructId_t structId;
} variableReferenceType;

typedef struct argumentSignature {
	const char *name;
	variableReferenceType type;
	uint8_t hasDefaultValue;
} argumentSignature;

typedef struct structSignature {
	argumentSignature *fields;
	uint8_t numFields;
} structSignature;

typedef struct sourceSignature {
	argumentSignature *args;
	variableReferenceType prodType;
	uint8_t numArgs;
} sourceSignature;

typedef struct pipeSignature {
	argumentSignature arg;
	variableReferenceType prodType;
} pipeSignature;

typedef struct sinkSignature {
	argumentSignature arg;
} sinkSignature;

typedef struct variableReference {
	const char *name;
	union {
		structSignature structSig;
		sourceSignature sourceSig;
		pipeSignature pipeSig;
		sinkSignature sinkSig;
	} extra;
	variableReferenceType type;
} variableReference;

typedef struct referenceStack {
	variableReference *refs;
	size_t capacity;
	size_t length;
} referenceStack;

typedef struct contextStack {
	int *values;
	size_t capacity;
	size_t length;
	size_t refStackOffset;
} contextStack;

#define STACK_INITIAL_CAPACITY 10

typedef struct compilationContext {
	plModule module;
	contextStack stack;
	referenceStack refStack;
} compilationContext;

static int recursivelyParseGlobalContent(astNodePtr tree, compilationContext *ctx);
static int addImport(const char *name, compilationContext *ctx);
static int addExport(const char *name, compilationContext *ctx);
static int parseStruct(astNodePtr tree, compilationContext *ctx);
static int parseNamedSource(astNodePtr tree, compilationContext *ctx);
static int parseNamedPipe(astNodePtr tree, compilationContext *ctx);
static int parseNamedSink(astNodePtr tree, compilationContext *ctx);
static void pushContextToStack(int nodeType, compilationContext *ctx);
static int popContextFromStack(compilationContext *ctx);
static const variableReference *resolveReference(const char *name, size_t len);

int compileSourceFile(const char *sourceFile) {
	int ret;
	astNodePtr programTree;
	compilationContext ctx;

	memset(&ctx,0,sizeof(compilationContext));
	moduleInit(&ctx.module);

	ret=recursivelyParseglobalContent(programTree,&ctx);
}

static int recursivelyParseGlobalContent(astNodePtr tree, compilationContext *ctx) {
	int ret;

	if ( !tree ) {
		return PL_ERROR_OK;
	}

	switch ( tree->nodeType ) {
		case ';':
		ret=recursivelyParseGlobalContent(tree->first,ctx);
		if ( ret != PL_ERROR_OK ) {
			return ret;
		}
		return recursivelyParseGlobalContent(tree->second,ctx);

		case IMPORT:
		return addImport((const char*)tree->first->first,ctx);

		case EXPORT:
		return addExport((const char*)tree->first->first,ctx);

		case STRUCT:
		return parseStruct(tree,ctx);

		case SOURCE:
		return parseNamedSource(tree,ctx);

		case PIPE:
		return parseNamedPipe(tree,ctx);

		case SINK:
		return parseNamedSink(tree,ctx);

		case MAIN:
		return parseMain(tree,ctx);

		default:
		ERROR_QUIT("Invalid nodeType: %i\n", tree->nodeType);
	}
}

static void pushContextToStack(int nodeType, compilationContext *ctx) {
	if ( ctx->stack.length >= ctx->stack.capacity ) {
		size_t newCapacity;

		newCapacity=( ctx->stack.capacity == 0 )? STACK_INITIAL_CAPACITY : (ctx->stack.capacity + 1)*5/4;
		ctx->stack.values=realloc(ctx->stack.values,sizeof(int)*newCapacity);
		if ( !ctx->stack.values ) {
			ERROR_QUIT("Failed to allocate %zu bytes", sizeof(int)*newCapacity);
		}
		ctx->stack.capacity=newCapacity;
	}

	ctx->stack.values[ctx->stack.length++]=nodeType;
}

static int popContextFromStack(compilationContext *ctx) {
	if ( ctx->stack.length == 0 ) {
		ERROR_QUIT("Tried to pop from an empty stack");
	}

	return ctx->stack.values[--ctx->stack.length];
}

static int addImport(const char *name, compilationContext *ctx) {

}

static int addExport(const char *name, compilationContext *ctx) {

}

static int parseStruct(astNodePtr tree, compilationContext *ctx) {

}

static int parseNamedSource(astNodePtr tree, compilationContext *ctx) {

}

static int parseNamedPipe(astNodePtr tree, compilationContext *ctx) {

}

static int parseNamedSink(astNodePtr tree, compilationContext *ctx) {

}

static int parseMain(astNodePtr tree, compilationContext *ctx) {

}

static const variableReference *resolveReference(const char *name, size_t len) {

}
