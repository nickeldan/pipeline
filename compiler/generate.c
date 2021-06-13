#include <string.h>

#include "table.h"

#include "generate.h"

typedef struct plReference {
    void *data;
    plLexicalLocation location;
    uint32_t flags;
} plReference;

typedef struct plSemanticContext {
    vasqLogger *logger;
    const char *file_name;
    plModule *module;
    plRefTable **stack;
    size_t stack_capacity;
    size_t stack_size;
} plSemanticContext;

#define STACK_INITIAL_CAPACITY 10

#define PL_REF_FLAG_OBJECT 0x80000000
#define PL_REF_FLAG_TYPE   0x40000000
#define PL_REF_FLAG_SOURCE 0x20000000
#define PL_REF_FLAG_PIPE   0x10000000
#define PL_REF_FLAG_SINK   0x08000000
#define PL_REF_FLAG_MODULE 0x04000000
#define PL_REF_FLAG_EXPORT 0x02000000

#define PL_REF_FLAG_CONST     0x00800000
#define PL_REF_FLAG_NULLABLE  0x00400000
#define PL_REF_FLAG_LOCALABLE 0x00200000
#define PL_REF_FLAG_NULL      0x00100000
#define PL_REF_FLAG_BLANK     0x00080000
#define PL_REF_FLAG_POS       0x00040000
#define PL_REF_FLAG_NEG       0x00020000
#define PL_REF_FLAG_ZERO      0x00010000
#define PL_REF_FLAG_TRUE      0x00008000
#define PL_REF_FLAG_FALSE     0x00004000

static const char *
refTypeName(uint32_t flags)
{
    flags &= 0xff000000;
    flags &= ~PL_REF_FLAG_EXPORT;

    switch (flags) {
    case PL_REF_FLAG_OBJECT: return "OBJECT";
    case PL_REF_FLAG_TYPE: return "TYPE";
    case PL_REF_FLAG_SOURCE: return "SOURCE";
    case PL_REF_FLAG_PIPE: return "PIPE";
    case PL_REF_FLAG_SINK: return "SINK";
    default: return "MODULE";
    }
}

static int
setupContextLogger(plSemanticContext *sem)
{
}

#if LL_USE == -1

static void
contextErrorNoLog(const plSemanticContext *sem, const plLexicalLocation *location, bool error,
                  const char *format, ...)
{
    char line[1024];
    va_list args;

    va_start(args, format);
    vasqSafeVsnprintf(line, sizeof(line), format, args);
    va_end(args);

    vasqRawLog(sem->logger, "%s%s:%u:%u: %s\n", error ? ERROR_STRING : WARNING_STRING, sem->file_name,
               location->line_no, location->column_no, line);
}
#define CONTEXT_ERROR(node, format, ...) \
    contextErrorNoLog(sem, &(node)->token.location, true, format, ##__VA_ARGS__)
#define CONTEXT_WARNING(node, format, ...) \
    contextErrorNoLog(sem, &(node)->token.location, false, format, ##__VA_ARGS__)

#else  // LL_USE == -1

static void
contextErrorLog(const char *function_name, unsigned int line_no, const plSemanticContext *sem,
                const plLexicalLocation *location, bool error, const char *format, ...)
{
    char line[1024];
    va_list args;

    va_start(args, format);
    vasqSafeVsnprintf(line, sizeof(line), format, args);
    va_end(args);

    vasqLogStatement(sem->logger, VASQ_LL_ERROR, __FILE__, function_name, line_no, "%s%s:%u:%u: %s",
                     error ? ERROR_STRING : WARNING_STRING, sem->file_name, location->line_no,
                     location->column_no, line);
}
#define CONTEXT_ERROR(node, format, ...) \
    contextErrorLog(__func__, __LINE__, sem &(node)->token.location, true, format, ##__VA_ARGS__)
#define CONTEXT_WARNING(node, format, ...) \
    contextErrorLog(__func__, __LINE__, sem, &(node)->token.location, false, format, ##__VA_ARGS__)

#endif  // LL_USE == -1

static plRefTable *
addTable(plSemanticContext *sem)
{
    plRefTable *new_table;

    new_table = plRefTableNew();
    if (new_table) {
        if (sem->stack_size == sem->stack_capacity) {
            size_t new_capacity;
            plRefTable **success;

            if (sem->stack_capacity == 0) {
                new_capacity = STACK_INITIAL_CAPACITY;
            }
            else {
                new_capacity = (sem->stack_capacity * 5) / 4;
            }
            success = VASQ_REALLOC(debug_logger, sem->stack, sizeof(*success) * new_capacity);
            if (!success) {
                plRefTableFree(new_table);
                return NULL;
            }

            sem->stack = success;
            sem->stack_capacity = new_capacity;
        }

        sem->stack[sem->stack_size++] = new_table;
    }

    return new_table;
}

static plReference *
newReference(void)
{
    plReference *ref;

    ref = VASQ_MALLOC(debug_logger, sizeof(*ref));
    if (ref) {
        *ref = (plReference){0};
    }
    return ref;
}

static int
storeReference(plRefTable *table, const char *symbol, uint32_t flags, void *data,
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

    for (size_t k = sem->stack_size; k > 0; k--) {
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

    size = sem->stack_size;
    for (size_t k = 0; k < size; k++) {
        plRefTableFree(sem->stack[k]);
    }

    free(sem->stack);
}

int
plCompileModule(plAstNode *tree, plModule **module, uint32_t compiler_flags)
{
}