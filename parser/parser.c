#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>

#include "parser.h"
#include "token.h"
#include "reference.h"

typedef struct {
	plToken_t *tokens;
	size_t length, size, offset;
} plTokenArray_t;

#define CURRENT_TOKEN(array) ((array)->tokens+(array)->offset)

#define PL_TOKEN_ARRAY_MIN_SIZE 100

typedef struct {
	void *data;
	plMarker_t marker;
} plParseFrame_t;

typedef struct {
	plParseFrame_t *frames;
	size_t length, size;
	plMarker_t currentContext;
} plParseStack_t;

#define CURRENT_FRAME(stack) ((stack)->frames+((stack)->length-1))

typedef struct {
	plModule_t *module;
	plParseStack_t *stack;
	plTokenArray_t *array;
} plParseCtx_t;

#define PL_PARSE_STACK_MIN_SIZE 30

static bool add_token_to_array(plTokenArray_t *array, const plToken_t *token);
static void clear_token_array(plTokenArray_t *array);
static bool push_to_parse_stack(plParseStack_t *stack, plMarker_t marker);
static void pop_from_parse_stack(plParseStack_t *stack);
static void clear_parse_stack(plParseStack_t *stack);
static void clear_parse_frame(plParseFrame_t *frame);
static plError_t recursively_parse_tokens(plParseCtx_t *ctx);
static plMarker_t print_parse_error(const plParseCtx_t *ctx, const char *format, ...);
static plToken_t *next_token(plParseCtx_t *ctx);
static plError_t parse_argument_list(plParseCtx_t *ctx, plNameReference_t **arguments);
static plError_t parse_output_type(plParseCtx_t *ctx, plNameReference_t **output);
static bool name_in_scope(const char *name, const plParseCtx_t *ctx);
static plError_t resolve_name(plParseCtx_t *ctx, plNameReference_t *reference);
static plError_t parse_literal(plParseCtx_t *ctx, plObject_t **object);
static bool literal_matches_reference(const plObject_t *object, const plNameReference_t *reference);
static plError_t parse_import(plParseCtx_t *ctx);
static plError_t parse_export(plParseCtx_t *ctx);
static plError_t parse_directive(plParseCtx_t *ctx);
static plError_t parse_source_header(plParseCtx_t *ctx);
static plError_t parse_pipe_header(plParseCtx_t *ctx);
static plError_t parse_sink_header(plParseCtx_t *ctx);
static plError_t parse_local_header(plParseCtx_t *ctx);
static plError_t parse_struct(plParseCtx_t *ctx);

plError_t parse_source_code(const char *path, plModule_t *module) {
	plError_t ret=PL_ERROR_OK;
	plFileReader_t reader;
	plTokenArray_t array;
	plParseStack_t stack;
	plParseCtx_t ctx;

	if ( !init_reader(&reader,path) ) {
		return PL_ERROR_FILE_ACCESS;
	}

	memset(&array,0,sizeof(plTokenArray_t));

	while ( true ) {
		plToken_t token;

		read_next_token(&reader,&token);

		if ( token.marker == PL_MARKER_EOF ) {
			if ( !add_token_to_array(&array,&token) ) {
				ret=PL_ERROR_MEMORY;
			}
			break;
		}
		else if ( token.marker == PL_MARKER_READ_FAILURE ) {
			ret=PL_ERROR_FILE_READ;
			break;
		}
		else if ( token.marker == PL_MARKER_MALLOC_FAILURE ) {
			ret=PL_ERROR_MEMORY;
			break;
		}
		else if ( TERMINAL_MARKER(token.marker) ) {
			fprintf(stderr,"%s: line %u\n", path, token.lineNo);
			fprintf(stderr,"Lexical error: %s\n", marker_name(token.marker));

			ret=PL_ERROR_LEX;
			break;
		}

		if ( !add_token_to_array(&array,&token) ) {
			clear_token(&token);

			ret=PL_ERROR_MEMORY;
			break;
		}
	}

	close_reader(&reader);

	if ( ret != PL_ERROR_OK ) {
		clear_token_array(&array);

		return ret;
	}

	if ( !set_module_path(module,path) ) {
		clear_token_array(&array);

		return PL_ERROR_MEMORY;
	}

	memset(&stack,0,sizeof(plParseStack_t));
	stack.currentContext=PL_MARKER_MODULE;

	ctx.module=module;
	ctx.stack=&stack;
	ctx.array=&array;

	ret=recursively_parse_tokens(&ctx);

	clear_token_array(&array);
	clear_parse_stack(&stack);

	return ret;
}

static bool add_token_to_array(plTokenArray_t *array, const plToken_t *token) {
	if ( array->size == 0 ) {
		array->tokens=malloc(sizeof(plToken_t)*PL_TOKEN_ARRAY_MIN_SIZE);
		if ( !array->tokens ) {
			return false;
		}
	}
	else if ( array->length == array->size ) {
		size_t newSize;
		plToken_t *success;

		newSize=array->size*5/4;
		success=realloc(array->tokens,sizeof(plToken_t)*newSize);
		if ( !success ) {
			return false;
		}

		array->tokens=success;
		array->size=newSize;
	}

	memcpy(array->tokens+array->length,token,sizeof(plToken_t));
	array->length++;

	return false;
}

static void clear_token_array(plTokenArray_t *array) {
	if ( array->tokens ) {
		for (size_t k=0; k<array->length; k++) {
			clear_token(array->tokens+k);
		}

		free(array->tokens);

		memset(array,0,sizeof(plTokenArray_t));
	}
}

static bool push_to_parse_stack(plParseStack_t *stack, plMarker_t marker) {
	if ( stack->size == 0 ) {
		stack->frames=malloc(sizeof(plParseFrame_t)*PL_PARSE_STACK_MIN_SIZE);
		if ( !stack->frames ) {
			return false;
		}
	}
	else if ( stack->length == stack->size ) {
		size_t newSize;
		plParseFrame_t *success;

		newSize=stack->size*5/4;
		success=realloc(stack->frames,sizeof(plParseFrame_t)*newSize);
		if ( !success ) {
			return false;
		}

		stack->frames=success;
		stack->size=newSize;
	}

	stack->frames[stack->length].data=NULL;
	stack->frames[stack->length].marker=marker;
	stack->length++;

	return true;
}

static void pop_from_parse_stack(plParseStack_t *stack) {
	if ( stack->length == 0 ) {
		return;
	}

	clear_parse_frame(stack->frames+(stack->length-1));
	stack->length--;
}

static void clear_parse_stack(plParseStack_t *stack) {
	if ( stack->frames ) {
		for (size_t k=0; k<stack->length; k++) {
			clear_parse_frame(stack->frames+k);
		}

		free(stack->frames);
	}

	memset(stack,0,sizeof(plParseStack_t));
}

static void clear_parse_frame(plParseFrame_t *frame) {
	switch ( frame->marker ) {
		case PL_MARKER_SOURCE:
		case PL_MARKER_PIPE:
		case PL_MARKER_SINK:
		clear_function_ctx((plFunctionCtx_t*)frame->data);
		break;
	}
}

static plError_t recursively_parse_tokens(plParseCtx_t *ctx) {
	while ( true ) {
		plError_t ret;
		plToken_t *token;
		plMarker_t frameMarker;

		token=CURRENT_TOKEN(ctx->array);

		switch ( token->marker ) {
			case PL_MARKER_SOURCE:
			case PL_MARKER_PIPE:
			case PL_MARKER_SINK:
			case PL_MARKER_LOCAL:
			case PL_MARKER_STRUCT:
			ctx->stack->currentContext=token->marker;
			case PL_MARKER_IF:
			case PL_MARKER_WHILE:
			frameMarker=token->marker;
			break;
			case PL_MARKER_EOF:
			if ( ctx->stack->currentContext == PL_MARKER_MODULE ) {
				return PL_ERROR_OK;
			}
			return print_parse_error(ctx,"Unexpected end of file found while parsing a %s.\n", marker_name(ctx->stack->currentContext));
			case PL_MARKER_EIF:
			case PL_MARKER_ELSE:
			return print_parse_error(ctx,"%s found without corresponding %s.\n", marker_name(token->marker), marker_name(PL_MARKER_IF));
			default:
			frameMarker=PL_MARKER_DIRECTIVE;
			break;
		}

		if ( ctx->stack->currentContext == PL_MARKER_MODULE ) {
			if ( frameMarker == PL_MARKER_IF || frameMarker == PL_MARKER_WHILE || frameMarker == PL_MARKER_LOCAL ) {
				return print_parse_error(ctx,"Illegal %s found in %s context.\n", marker_name(frameMarker), marker_name(PL_MARKER_MODULE));
			}

			if ( token->marker == PL_MARKER_IMPORT ) {
				ret=parse_import(ctx);
				if ( ret != PL_RET_OK ) {
					return ret;
				}
				continue;
			}

			if ( token->marker == PL_MARKER_EXPORT ) {
				ret=parse_export(ctx);
				if ( ret != PL_RET_OK ) {
					return ret;
				}
				continue;
			}
		}

		if ( token->maker == PL_MARKER_IMPORT || frameMarker == PL_MARKER_EXPORT ) {
			if ( ctx->stack->currentContext != PL_MARKER_MODULE ) {
				return print_parse_error(ctx,"Illegal %s found in %s context.\n", marker_name(frameMarker))
			}
		}

		if ( !push_to_parse_stack(stack,frameMarker) ) {
			return PL_ERROR_MEMORY;
		}

		if ( frameMarker == PL_MARKER_DIRECTIVE ) {
			ret=parse_directive(ctx);
		}
		else {
			switch ( token->marker ) {
				case PL_MARKER_SOURCE: ret=parse_source_header(ctx); break;
				case PL_MARKER_PIPE: ret=parse_pipe_header(ctx); break;
				case PL_MARKER_SINK: ret=parse_sink_header(ctx); break;
				case PL_MARKER_LOCAL: ret=parse_local_header(ctx); break;
				case PL_MARKER_STRUCT: ret=parse_struct(ctx); break;
				default: ret=parse_if_while_header(ctx); break;
			}
		}

		if ( ret != PL_ERROR_OK ) {
			return ret;
		}
	}
}

static plMarker_t print_parse_error(const plParseCtx_t *ctx, const char *format, ...) {
	va_list args;

	fprintf(stderr,"%s: line %lu\n", module_path(ctx->module), CURRENT_TOKEN(ctx->array)->lineNo);
	fprintf(stderr,"Error: ");
	va_start(args,format);
	vfprintf(stderr,format,args);

	return PL_ERROR_GRAMMAR;
}

static plToken_t *next_token(plParseCtx_t *ctx) {
	plToken_t *token;

	ctx->array->offset++;
	token=CURRENT_TOKEN(ctx->array);
	if ( token->marker == PL_MARKER_EOF ) {
		print_parse_error(ctx,"Unexpected end of file found while parsing a %s.\n", marker_name(ctx->stack->currentContext));
	}

	return token;
}

static plError_t parse_argument_list(plParseCtx_t *ctx, plNameReference_t **arguments) {
	plError_t ret=PL_ERROR_GRAMMAR;
	plNameReference_t *lastArgument;
	bool has_ellipsis=false;

	*arguments=lastArgument=NULL;

	while ( true ) {
		plToken_t *token;
		char *name;
		plNameReference_t *reference;

		reference=calloc(1,sizeof(plNameReference_t));
		if ( !reference ) {
			ret=PL_ERROR_MEMORY;
			goto error;
		}

		token=next_token(ctx);
		if ( token->marker == PL_MARKER_EOF ) {
			free(reference);
			goto error;
		}

		if ( token->marker == PL_MARKER_ELLIPSIS ) {
			if ( has_ellipsis ) {
				print_parse_error(ctx,"More than one %s found in argument list.\n", marker_name(PL_MARKER_ELLIPSIS));
				free(reference);
				goto error;
			}
			has_ellipsis=true;
			reference->marker=PL_MARKER_ELLIPSIS;

			if ( lastArgument ) {
				lastArgument->next=reference;
				lastArgument=reference;

				if ( next_token(ctx)->marker != PL_MARKER_CLOSE_PARENS ) {
					print_parse_error(ctx,"Expected %s following %s.\n", marker_name(PL_MARKER_CLOSE_PARENS), marker_name(PL_MARKER_ELLIPSIS));
					goto error;
				}

				if ( next_token(ctx)->marker == PL_MARKER_EOF ) {
					goto error;
				}

				break;
			}
			else {
				*arguments=lastArgument=reference;
			}
		}
		else {
			if ( token->marker != PL_MARKER_NAME ) {
				print_parse_error(ctx,"Expected variable name in argument list.\n");
				free(reference);
				goto error;
			}
			name=(char*)token->data;
			if ( name_in_scope(name,ctx) ) {
				print_parse_error(ctx,"The name %s is already in scope.\n", name);
				free(reference);
				goto error;
			}

			reference->marker=PL_MARKER_NAME;

			token=next_token(ctx);
			if ( token->marker == PL_MARKER_EOF ) {
				free(reference);
				goto error;
			}
			if ( token->marker == PL_MARKER_COLON ) {
				plNameReference_t predicateReference;

				if ( next_token(ctx)->marker == PL_MARKER_EOF ) {
					free(reference);
					goto error;
				}

				ret=resolve_name(ctx,&predicateReference);
				if ( ret != PL_ERROR_OK ) {
					free(reference);
					goto error;
				}

				if ( predicateReference.marker != PL_MARKER_PRED ) {
					print_parse_error(ctx,"Expected %s in argument list.\n", marker_name(PL_MARKER_PRED));
					free(reference);
					goto error;
				}

				reference->name=strdup(predicateReference.name);
				reference->type=predicateReference.type;
				reference->moduleId=predicateReference.moduleId;
				reference->structId=predicateReference.structId;
			}
			else {
				reference->name=strdup(name);
				reference->type=PL_PRED_ANY;
				reference->moduleId=PL_BUILTIN_MODULE;
				reference->structId=PL_NOT_STRUCT;
			}

			if ( !reference->name ) {
				free(reference);
				ret=PL_ERROR_MEMORY;
				goto error;
			}
		}

		if ( lastArgument ) {
			lastArgument->next=reference;
		}
		else {
			*arguments=reference;
		}
		lastArgument=reference;

		token=next_token(ctx);
		if ( token->marker == PL_MARKER_EOF) {
			goto error;
		}

		if ( token->marker == PL_MARKER_ASSIGNMENT ) {
			plError_t ret;

			if ( reference->marker == PL_MARKER_ELLIPSIS ) {
				print_parse_error(ctx,"Unexpected %s following %s.\n", marker_name(PL_MARKER_ASSIGNMENT), marker_name(PL_MARKER_ELLIPSIS));
				goto error;
			}

			if ( ctx->stack->currentContext == PL_MARKER_SINK ) {
				print_parse_error(ctx,"Default values are not allowed for a %s.\n", marker_name(PL_MARKER_SINK));
				goto error;
			}

			token=next_token(ctx);
			if ( token->marker == PL_MARKER_EOF ) {
				goto error;
			}

			ret=parse_literal(ctx,&reference->data);
			if ( ret != PL_ERROR_OK ) {
				goto error;
			}

			if ( !literal_matches_reference((plObject_t*)&reference->data,reference) ) {
				free_object((plObject_t*)&reference->data);
				reference->data=NULL;
				goto error;
			}

			token=next_token(ctx);
			if ( token->marker == PL_MARKER_EOF ) {
				goto error;
			}
		}

		if ( token->marker == PL_MARKER_COMMA ) {
			continue;
		}
		else if ( token->marker == PL_MARKER_CLOSE_PARENS ) {
			if ( next_token(ctx)->marker == PL_MARKER_EOF ) {
				goto error;
			}
			break;
		}
		else {
			print_parse_error(ctx,"Unexpected %s in argument list.\n", marker_name(token->marker));
			goto error;
		}
	}

	return PL_ERROR_OK;

	error:

	while ( *arguments ) {
		plNameReference_t *temp;

		temp=*arguments;
		*arguments=(*arguments)->next;
		clear_name_reference(temp);
	}

	return ret;
}

static plError_t parse_output_type(plParseCtx_t *ctx, plNameReference_t **output) {
	plError_t ret=PL_ERROR_GRAMMAR;
	plNameReference_t *lastOutput;
	bool has_ellipsis=false;

	*output=lastOutput=NULL;

	while ( true ) {
		plMarker_t marker;
		plNameReference_t *reference;

		marker=next_token(ctx)->marker;
		if ( marker == PL_MARKER_EOF ) {
			goto error;
		}

		referene=calloc(1,sizeof(plNameReference_t));
		if ( !reference ) {
			ret=PL_ERROR_MEMORY;
			goto error;
		}

		if ( marker == PL_MARKER_ELLIPSIS ) {
			if ( has_ellipsis ) {
				print_parse_error(ctx,"More than one %s found in output type.\n", marker_name(PL_MARKER_ELLIPSIS));
				free(reference);
				goto error;
			}
			has_ellipsis=true;
			reference->marker=PL_MARKER_ELLIPSIS;

			if ( lastOutput ) {
				lastOutput->next=reference;
				lastOutput=reference;

				if ( next_token(ctx)->marker != PL_MARKER_OPEN_BRACE ) {
					print_parse_error(ctx,"Expected %s following %s.\n", marker_name(PL_MARKER_OPEN_BRACE), marker_name(PL_MARKER_ELLIPSIS));
					goto error;
				}

				break;
			}
			else {
				*output=lastOutput=reference;
			}
		}
		else {
			ret=resolve_name(ctx,reference);
			if ( ret != PL_ERROR_OK ) {
				free(reference);
				goto error;
			}

			if ( reference->marker != PL_MARKER_PRED ) {
				print_parse_error(ctx,"Expected %s in output type.\n", marker_name(PL_MARKER_PRED));
				free(reference);
				goto error;
			}
		}

		if ( lastOutput ) {
			lastOutput->next=reference;
		else {
			*output=reference;
		}
		lastOutput=reference;

		marker=next_token(ctx)->marker;
		if ( marker == PL_MARKER_EOF ) {
			goto error;
		}
		else if ( marker == PL_MARKER_OPEN_BRACE ) {
			if ( next_token(ctx)->marker == PL_MARKER_EOF ) {
				goto error;
			}
			break;
		}
		else if ( marker != PL_MARKER_COMMA ) {
			print_parse_error(ctx,"Unexpected %s following output type.\n", marker_name(marker));
			goto error;
		}
	}

	return PL_ERROR_OK;

	error:

	while ( *output ) {
		plNameReference_t *temp;

		temp=*output;
		*output=(*output)->next;
		clear_name_reference(temp);
	}

	return ret;
}

static bool name_in_scope(const char *name, const plParseCtx_t *ctx) {

}

static plError_t resolve_name(plParseCtx_t *ctx, plNameReference_t *reference) {

}

static plError_t parse_literal(plParseCtx_t *ctx, plObject_t **object) {

}

static bool literal_matches_reference(const plObject_t *object, const plNameReference_t *reference) {

}

static plError_t parse_source_header(plParseCtx_t *ctx) {
	plError_t ret=PL_ERROR_GRAMMAR;
	plFunctionCtx_t *functionCtx;
	plNameReference_t *lastArgument=NULL;
	plToken_t *token;

	token=next_token(ctx);
	if ( token->marker == PL_MARKER_EOF ) {
		return PL_ERROR_GRAMMAR;
	}

	functionCtx=calloc(1,sizeof(plFunctionCtx_t));
	if ( !functionCtx ) {
		return PL_ERROR_MEMORY;
	}

	if ( token->marker == PL_MARKER_NAME ) {
		if ( ctx->stack->currentContext != PL_PARSE_MODULE ) {
			print_parse_error(ctx,"Named %ss are only allowed in a %s context.\n", marker_name(PL_MARKER_SOURCE), marker_name(PL_MARKER_MODULE));
			goto error;
		}

		if ( name_in_scope((char*)token->data,ctx) ) {
			print_parse_error(ctx,"The name %s is already in scope.\n", (char*)token->data);
			goto error;
		}

		functionCtx->name=strdup(token->data);
		if ( !functionCtx->name ) {
			ret=PL_ERROR_MEMORY;
			goto error;
		}

		token=next_token(ctx);
		if ( token->marker == PL_MARKER_EOF ) {
			goto error;
		}
	}
	else if ( ctx->stack->currentContext == PL_PARSE_MODULE ) {
		free(functionCtx);
		return print_parse_error(ctx,"Anonymous %ss are not allowed in a %s context.\n", marker_name(PL_MARKER_SOURCE), marker_name(PL_MARKER_MODULE));
	}

	if ( token->marker == PL_MARKER_OPEN_PARENS ) {
		ret=parse_argument_list(ctx,&functionCtx->arguments);
		if ( ret != PL_ERROR_OK ) {
			goto error;
		}
		token=CURRENT_TOKEN(ctx->array);
	}

	if ( token->marker != PL_MARKER_ARROW ) {
		print_parse_error(ctx,"Expected output type following %s header.\n", marker_name(PL_MARKER_SOURCE));
		goto error;
	}

	ret=parse_output_type(ctx,&functionCtx->output);
	if ( ret != PL_ERROR_OK ) {
		goto error;
	}
	token=CURRENT_TOKEN(ctx->array);

	if ( token->marker != PL_MARKER_OPEN_BRACE ) {
		print_parse_error(ctx,"Expected %s following %s header.\n", marker_name(PL_MARKER_OPEN_BRACE), marker_name(PL_MARKER_SOURCE));
		goto error;
	}

	if ( next_token(ctx)->marker == PL_MARKER_EOF ) {
		goto error;
	}

	CURRENT_FRAME(ctx->stack)->data=functionCtx;
	return PL_ERROR_OK;

	error:

	clear_function_ctx(functionCtx);
	return ret;
}