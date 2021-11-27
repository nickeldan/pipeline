#ifndef PIPELINE_COMPILER_GENERATE_INTERNAL_H
#define PIPELINE_COMPILER_GENERATE_INTERNAL_H

// util header files
#include "table.h"
#include "util.h"

// vm header files
#include "module.h"

// compiler/parser header files
#include "ast.h"

#include "reference.h"

typedef struct plSemanticContext {
    vasqLogger *logger;
    const char *file_name;
    plModule *module;
    plRefTable **stack;
    size_t stack_capacity;
    size_t stack_size;
    unsigned int num_errors;
    uint32_t compiler_flags;
} plSemanticContext;

void HIDDEN_SYMBOL
plContextError(const char *file_name, const char *function_name, unsigned int line_no,
               plSemanticContext *sem, const plLexicalLocation *location, vasqLogLevel_t level,
               const char *format, ...);
#define CONTEXT_ERROR(node, format, ...)                                                               \
    plContextError(__FILE__, __func__, __LINE__, sem, &(node)->header.location, VASQ_LL_ERROR, format, \
                   ##__VA_ARGS__)
#define CONTEXT_WARNING(node, format, ...)                                                               \
    plContextError(__FILE__, __func__, __LINE__, sem, &(node)->header.location, VASQ_LL_WARNING, format, \
                   ##__VA_ARGS__)

plReference HIDDEN_SYMBOL *
plFindReference(const plSemanticContext *sem, const char *symbol, size_t *idx);

plReference HIDDEN_SYMBOL *
plResolveExtendedName(const plSemanticContext *sem, const plAstNode *node, size_t *idx);

plRefTable HIDDEN_SYMBOL *
plAddTable(plSemanticContext *sem);

int HIDDEN_SYMBOL
plCompileImport(plSemanticContext *sem, plAstNode *node);

int HIDDEN_SYMBOL
plCompileExport(plSemanticContext *sem, plAstNode *node);

int HIDDEN_SYMBOL
plCompileOpaque(plSemanticContext *sem, plAstNode *node);

#endif  // PIPELINE_COMPILER_GENERATE_INTERNAL_H
