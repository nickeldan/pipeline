#ifndef __PIPELINE_FRAMES_H__
#define __PIPELINE_FRAMES_H__

#include "token.h"
#include "../pipeline/object.h"

typedef struct {
	void *data;
	plMarker_t marker;
} plParseFrame_t;

typedef struct _plNameReference_t {
	struct _plNameReference_t *next;
	char *name;
	void *data;
	plModuleId_t moduleId;
	plStructId_t structId;
	plMarker_t marker;
	plObjectType_t type;
} plNameReference_t;

typedef struct {
	plNameReference_t *arguments;
	plNameReference_t *output;
	char *name;
} plFunctionCtx_t;

typedef struct {
	plNameReference_t *arguments;
	plNameReference_t *output;
} plAnonFunctionCtx_t;

void clear_name_reference(plNameReference_t *reference);
void clear_function_ctx(plFunctionCtx_t *ctx);
void clear_anon_function_ctx(plAnonFunctionCtx_t *ctx);

#endif // __PIPELINE_FRAMES_H__