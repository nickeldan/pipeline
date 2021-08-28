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
    uint32_t compiler_flags;
} plSemanticContext;

void
plContextError(const char *file_name, const char *function_name, unsigned int line_no,
               const vasqLogger *logger, const plLexicalLocation *location, bool error, const char *format,
               ...);
#define CONTEXT_ERROR(node, format, ...)                                                             \
    plContextError(__FILE__, __func__, __LINE__, sem->logger, &(node)->token.location, true, format, \
                   ##__VA_ARGS__)
#define CONTEXT_WARNING(node, format, ...)                                                            \
    plContextError(__FILE__, __func__, __LINE__, sem->logger, &(node)->token.location, false, format, \
                   ##__VA_ARGS__)

plReference *
plFindReference(const plSemanticContext *sem, const char *symbol, size_t *idx);

plReference *
plResolveExtendedName(const plSemanticContext *sem, const plAstNode *node, size_t *idx);

plRefTable *
plAddTable(plSemanticContext *sem);

int
plCompileImport(plSemanticContext *sem, plAstNode *node);

int
plCompileExport(plSemanticContext *sem, plAstNode *node);

int
plCompileTypeDecl(plSemanticContext *sem, plAstNode *node);

#endif  // PIPELINE_COMPILER_GENERATE_INTERNAL_H
