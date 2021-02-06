#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "vasq/safe_snprintf.h"

#include "plObject.h"

#include "ir.h"
#include "module.h"
#include "table.h"
#include "token.h"

typedef struct plReference {
    void *data;
    plLexicalLocation location;
    uint32_t flags;
} plReference;

#define PL_REF_FLAG_OBJECT 0x80000000
#define PL_REF_FLAG_SOURCE 0x40000000
#define PL_REF_FLAG_PIPE   0x20000000
#define PL_REF_FLAG_SINK   0x10000000
#define PL_REF_FLAG_MODULE 0x08000000
#define PL_REF_FLAG_EXPORT 0x04000000

#define PL_REF_FLAG_CONST     0x00800000
#define PL_REF_FLAG_NULLABLE  0x00400000
#define PL_REF_FLAG_LOCALABLE 0x00200000
#define PL_REF_FLAG_NULL      0x00100000
#define PL_REF_FLAG_BLANK     0x00080000
#define PL_REF_FLAG_NONNEG    0x00040000
#define PL_REF_FLAG_NONPOS    0x00020000
#define PL_REF_FLAG_ZERO      0x00010000
#define PL_REF_FLAG_TRUE      0x00008000
#define PL_REF_FLAG_FALSE     0x00004000

struct plIr {
};

typedef struct plSemanticContext {
    plIr *ir;
    const char *file_name;
    plWordTable **stack;
    size_t stack_capacity;
    size_t stack_size;
} plSemanticContext;

#define STACK_INITIAL_CAPACITY 10

static const char *
refTypeName(uint32_t flags)
{
    flags &= 0xff000000;
    flags &= ~PL_REF_FLAG_EXPORT;

    switch (flags) {
    case PL_REF_FLAG_OBJECT: return "OBJECT";
    case PL_REF_FLAG_SOURCE: return "SOURCE";
    case PL_REF_FLAG_PIPE: return "PIPE";
    case PL_REF_FLAG_SINK: return "SINK";
    default: return "MODULE";
    }
}

#if LL_USE == -1

static void
contextErrorNoLog(const char *file_name, const plLexicalLocation *location, bool error, const char *format,
                  ...)
{
    char line[1024];
    va_list args;

    va_start(args, format);
    vasqSafeVsnprintf(line, sizeof(line), format, args);
    va_end(args);

    vasqRawLog("%s%s:%u:%u: %s\n", error ? ERROR_STRING : WARNING_STRING, file_name, location->line_no,
               location->column_no, line);
}
#define CONTEXT_ERROR(node, format, ...) \
    contextErrorNoLog(sem->file_name, &(node)->token.location, true, format, ##__VA_ARGS__)
#define CONTEXT_WARNING(node, format, ...) \
    contextErrorNoLog(sem->file_name, &(node)->token.location, false, format, ##__VA_ARGS__)

#else  // LL_USE == -1

static void
contextErrorLog(const char *function_name, unsigned int line_no, const char *file_name,
                const plLexicalLocation *location, bool error, const char *format, ...)
{
    char line[1024];
    va_list args;

    va_start(args, format);
    vasqSafeVsnprintf(line, sizeof(line), format, args);
    va_end(args);

    vasqLogStatement(VASQ_LL_ERROR, __FILE__, function_name, line_no, "%s%s:%u:%u: %s",
                     error ? ERROR_STRING : WARNING_STRING, file_name, location->line_no,
                     location->column_no, line);
}
#define CONTEXT_ERROR(node, format, ...) \
    contextErrorLog(__func__, __LINE__, sem->file_name, &(node)->token.location, true, format, ##__VA_ARGS__)
#define CONTEXT_WARNING(node, format, ...)                                                      \
    contextErrorLog(__func__, __LINE__, sem->file_name, &(node)->token.location, false, format, \
                    ##__VA_ARGS__)

#endif  // LL_USE == -1

static plWordTable *
addTable(plSemanticContext *sem)
{
    plWordTable *new_table;

    new_table = plWordTableNew();
    if (new_table) {
        if (sem->size == sem->capacity) {
            size_t new_capacity;
            plWordTable **success;

            if (sem->capacity == 0) {
                new_capacity = STACK_INITIAL_CAPACITY;
            }
            else {
                new_capacity = (sem->capacity * 5) / 4;
            }
            success = VASQ_REALLOC(sem->stack, sizeof(*success) * new_capacity);
            if (!success) {
                plWordTableFree(new_table);
                return NULL;
            }

            sem->stack = success;
            sem->capacity = new_capacity;
        }

        sem->stack[sem->size++] = new_table;
    }

    return new_table;
}

static plReference *
newReference(void)
{
    plReference *ref;

    ref = VASQ_MALLOC(sizeof(*ref));
    if (ref) {
        *ref = (plReference){0};
    }
    return ret;
}

static int
storeReference(plWordTable *table, const char *symbol, uint32_t flags, void *data,
               const plLexicalLocation *location)
{
    plReference *ref;

    ref = newReference();
    if (!ref) {
        return PL_RET_OUT_OF_MEMORY;
    }

    ref->flags = flags;
    ref->data = data;
    memcpy(&ref->location, location, sizeof(*location));

    if (!plRegisterWord(table, symbol, ref)) {
        free(ref);
        return PL_RET_OUT_OF_MEMORY;
    }

    return PL_RET_OK;
}

static plReference *
findReference(const plSemanticContext *sem, const char *symbol, size_t *idx)
{
    plReference *ref;

    for (size_t k = sem->size; k > 0; k--) {
        if (plLookupName(sem->stack[k - 1], &ref)) {
            if (idx) {
                *idx = k;
            }
            return ref;
        }
    }

    return NULL;
}

static void
destroyStack(plSemanticContext *sem)
{
    size_t size;

    size = sem->size;
    for (size_t k = 0; k < size; k++) {
        plWordTableFree(sem->stack[k]);
    }

    free(sem->stack);
}

static int
generateGlobalIr(plSemanticContext *sem, const plAstNode *tree)
{
    int ret;
    const plAstMaxSplitNode *splitter = (const plAstMaxSplitNode *)tree;

    if (!tree) {
        return PL_RET_OK;
    }

    switch (tree->token.marker) {
        const char *name;
        plReference *ref;
        plModule *module;

    case PL_MARKER_SEMICOLON:
        for (int k = 0; k < 2; k++) {
            ret = generateGlobalIr(sem, splitter->nodes[k]);
            if (ret != PL_RET_OK) {
                return ret;
            }
        }
        return PL_RET_OK;

    case PL_MARKER_EXPORT:
        name = splitter->nodes[0].token.ctx.name;
        ref = findReference(sem, name, NULL);
        if (ref) {
            if (ref->flags & PL_REF_FLAG_EXPORT) {
                CONTEXT_WARNING(tree, "%s was already exported on line %u.", name, ref->location.line_no);
                return PL_RET_OK;
            }
            else {
                CONTEXT_ERROR(tree, "%s was already defined as %s on line %u.", name,
                              refTypeName(ref->flags), ref->location.line_no);
                return PL_RET_BAD_DATA;
            }
        }

        return storeReference(sem->stack[0], name, PL_REF_FLAG_EXPORT, NULL,
                              &splitter->nodes[0].token.location);

    case PL_MARKER_IMPORT:
        name = splitter->nodes[0].token.ctx.name;
        ref = findReference(sem, name, NULL);
        if (ref) {
            if (ref->flags & PL_REF_FLAG_MODULE) {
                CONTEXT_WARNING(tree, "%s ws already imported on line %u.", name, ref->location.line_no);
                return PL_RET_OK;
            }
            else {
                CONTEXT_ERROR("%s was already defined as %s on line %u.", name, refTypeName(ref->flags),
                              ref->location.line_no);
                return PL_RET_BAD_DATA;
            }
        }

        ret = plModuleLoad(name, &module);
        if (ret != PL_RET_OK) {
            CONTEXT_ERROR("Failed to load module '%s'.", name);
            return ret;
        }

        ret = storeReference(sem->stack[0], name, PL_REF_FLAG_MODULE, module,
                             &splitter->nodes[0].token.location);
        if (ret != PL_RET_OK) {
            plModuleFree(module);
        }
        return ret;
    }
}

int
plIrGenerate(const char *file_name, const plAstNode *tree, plIr **ir)
{
    int ret;
    const char *symbol;
    plSemanticContext sem = {.file_name = file_name};
    plWordTableIterator iterator;
    plReference ref;

    if (!file_name || !tree || !ir) {
        VASQ_ERROR("The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    *ir = sem.ir = VASQ_MALLOC(sizeof(**ir));
    if (!*ir) {
        return PL_RET_OUT_OF_MEMORY;
    }
    **ir = (plIr){0};

    if (!addTable(&sem)) {
        plIrFree(*ir);
        return PL_RET_OUT_OF_MEMORY;
    }

    ret = generateGlobalIr(&sem, tree);
    if (ret != PL_RET_OK) {
        goto error;
    }

    plWordTableIteratorInit(&iterator, sem->stack[0]);
    while ((symbol = plWordTableIterate(&iterator, &ref))) {
        if (ref.flags == PL_REF_FLAG_EXPORT) {
#if LL_USE == -1
            contextErrorNoLog(file_name, &ref.location, true, "Exported symbol '%s' not resolved.", symbol);
#else
            contextErrorLog(__func__, __LINE__, file_name, &ref.location, true,
                            "Exported symbol '%s' not resolved.", symbol);
#endif

            ret = PL_RET_BAD_DATA;
            goto error;
        }
    }

    goto done;

error:

    plIrFree(*ret);

done:

    destroyStack(&sem);

    return ret;
}

void
plIrFree(plIr *ir)
{
}