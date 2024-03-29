#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vasq/safe_snprintf.h"

// util header files
#include "table.h"

#include "builtins.h"
#include "generate.h"
#include "generateInternal.h"

#define STACK_INITIAL_CAPACITY 10

static void
semanticLogProcessor(void *user_data, size_t idx, vasqLogLevel_t level, char **dst, size_t *remaining)
{
    (void)idx;
    plSemanticContext *sem = user_data;

    vasqIncSnprintf(dst, remaining, "%s%s:", (level == VASQ_LL_WARNING) ? WARNING_STRING : ERROR_STRING,
                    sem->file_name);
}

static void
destroyStack(plSemanticContext *sem)
{
    size_t size;

    size = sem->stack_size;
    for (size_t k = 0; k < size; k++) {
        plRefTableFree(sem->stack[k]);
    }

    free(sem->stack);
}

static int
initSemanticContext(plSemanticContext *sem)
{
    int ret;
    vasqLoggerOptions options = {.processor = semanticLogProcessor, .user = sem};

    for (int k = 0; k < 2; k++) {  // built-in space and global space
        if (!plAddTable(sem)) {
            ret = PL_RET_OUT_OF_MEMORY;
            goto error;
        }
    }

    ret = plAddBuiltinSymbols(sem->stack[0]);
    if (ret != PL_RET_OK) {
        goto error;
    }

    ret = vasqLoggerCreate(STDERR_FILENO, VASQ_LL_WARNING, "%x%M\n", &options, &sem->logger);
    if (ret != VASQ_RET_OK) {
        ret = plTranslateVasqRet(ret);
        goto error;
    }

    return PL_RET_OK;

error:

    destroyStack(sem);
    vasqLoggerFree(sem->logger);

    return ret;
}

static int
compileGlobalSpace(plSemanticContext *sem, plAstNode *tree)
{
    int ret;
    int (*func)(plSemanticContext *, plAstNode *);

    switch (tree->header.marker) {
    case PL_MARKER_SEMICOLON:
        ret = compileGlobalSpace(sem, AST_CHILD(tree, 0));
        if (ret != PL_RET_OK) {
            return ret;
        }
        func = compileGlobalSpace;
        tree = AST_CHILD(tree, 1);
        break;

    case PL_MARKER_IMPORT: func = plCompileImport; break;

    case PL_MARKER_EXPORT: func = plCompileExport; break;

    case PL_MARKER_OPAQUE: func = plCompileOpaque; break;

    case PL_MARKER_EXPORT_ALL: sem->compiler_flags |= PL_COMPILER_FLAG_EXPORT_ALL; return PL_RET_OK;

    case PL_MARKER_STRUCT: func = plCompileStructDefinition; break;

    case PL_MARKER_SOURCE:
    case PL_MARKER_PIPE:
    case PL_MARKER_SINK: func = plCompileFunction; break;

    default: VASQ_ERROR(debug_logger, "Invalid node type: %i.", tree->header.marker); return PL_RET_USAGE;
    }

    return func(sem, tree);
}

int
plGenerateModule(plAstNode *tree, const char *file_name, plModule *module, uint32_t compiler_flags)
{
    int ret;
    plSemanticContext sem = {.file_name = file_name, .module = module, .compiler_flags = compiler_flags};

    if (UNLIKELY(!tree || !file_name || !module)) {
        VASQ_ERROR(debug_logger, "tree, file_name, and module cannot be NULL");
        return PL_RET_USAGE;
    }

    ret = initSemanticContext(&sem);
    if (ret != PL_RET_OK) {
        return ret;
    }

    plModuleInit(module);

    ret = compileGlobalSpace(&sem, tree);

    destroyStack(&sem);
    vasqLoggerFree(sem.logger);
    return ret;
}

void
plContextError(const char *file_name, const char *function_name, unsigned int line_no,
               plSemanticContext *sem, const plLexicalLocation *location, vasqLogLevel_t level,
               const char *format, ...)
{
    char line[1024];
    va_list args;

    if (level <= VASQ_LL_ERROR) {
        sem->num_errors++;
    }

    va_start(args, format);
    vasqSafeVsnprintf(line, sizeof(line), format, args);
    va_end(args);

    vasqLogStatement(sem->logger, level, file_name, function_name, line_no, "%u:%u: %s", location->line_no,
                     location->column_no, line);
}

plReference *
plFindReference(const plSemanticContext *sem, const char *symbol, size_t *idx)
{
    if (UNLIKELY(!sem || !symbol)) {
        VASQ_ERROR(debug_logger, "sem and symbol cannot be NULL.");
        return NULL;
    }

    for (size_t k = sem->stack_size; k > 0; k--) {
        void *opaque;

        if (plLookupRef(sem->stack[k - 1], symbol, strlen(symbol), &opaque)) {
            if (idx) {
                *idx = k - 1;
            }
            return (plReference *)opaque;
        }
    }

    return NULL;
}

plReference *
plResolveExtendedName(const plSemanticContext *sem, const plAstNode *node, size_t *idx)
{
    plReference *ref;

    if (UNLIKELY(!sem || !node)) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return NULL;
    }

    if (node->header.marker == PL_MARKER_NAME) {
        return plFindReference(sem, plAstGetData((plAstNode *)node)->name, idx);
    }

    PLACEHOLDER();
    (void)ref;
    return NULL;
}

plRefTable *
plAddTable(plSemanticContext *sem)
{
    plRefTable *new_table;

    if (UNLIKELY(!sem)) {
        VASQ_ERROR(debug_logger, "sem cannot be NULL.");
        return NULL;
    }

    new_table = plRefTableNew();
    if (new_table) {
        if (sem->stack_size == sem->stack_capacity) {
            size_t new_capacity;

            if (sem->stack_capacity == 0) {
                new_capacity = STACK_INITIAL_CAPACITY;
            }
            else {
                new_capacity = (sem->stack_capacity * 3) / 2;
            }
            sem->stack = plSafeRealloc(sem->stack, sizeof(sem->stack) * new_capacity);
            sem->stack_capacity = new_capacity;
        }

        sem->stack[sem->stack_size++] = new_table;
    }

    return new_table;
}
