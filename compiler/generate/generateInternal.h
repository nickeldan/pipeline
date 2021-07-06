#ifndef PIPELINE_COMPILER_GENERATE_INTERNAL_H
#define PIPELINE_COMPILER_GENERATE_INTERNAL_H

#include "table.h"

#include "module.h"

#include "ast.h"
#include "definitions.h"
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
contextError(const char *file_name, const char *function_name, unsigned int line_no,
             const vasqLogger *logger, const plLexicalLocation *location, bool error, const char *format,
             ...);
#define CONTEXT_ERROR(node, format, ...)                                                           \
    contextError(__FILE__, __func__, __LINE__, sem->logger, &(node)->token.location, true, format, \
                 ##__VA_ARGS__)
#define CONTEXT_WARNING(node, format, ...)                                                          \
    contextError(__FILE__, __func__, __LINE__, sem->logger, &(node)->token.location, false, format, \
                 ##__VA_ARGS__)

plReference *
findReference(const plSemanticContext *sem, const char *symbol, size_t *idx);

plRefTable *
addTable(plSemanticContext *sem);

plReference *
newReference(void);

int
storeReference(plRefTable *table, const char *symbol, uint32_t flags, void *data,
               const plLexicalLocation *location);

int
compileImportExport(plSemanticContext *sem, plAstNode *node);

#endif  // PIPELINE_COMPILER_GENERATE_INTERNAL_H
