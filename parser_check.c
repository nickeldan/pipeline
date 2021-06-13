#include <unistd.h>

#include "vasq/logger.h"

#include "util.h"

#include "parser.h"

int
main(int argc, char **argv)
{
    int ret;
    FILE *f;
    const char *file_name;
    plWordTable *table;
    plAstNode *tree;

    ret = plSetupDebuggingLogger(LL_USE);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (argc == 1) {
        f = stdin;
        file_name = "stdin";
    }
    else {
        f = fopen(argv[1], "r");
        if (!f) {
            VASQ_PERROR(debug_logger, "fopen", errno);
            return PL_RET_NO_ACCESS;
        }
        file_name = argv[1];
    }

    ret = plFileParse(f, file_name, &tree, &table);
    if (ret == PL_RET_OK) {
        plAstPrint(tree, 0);
        plAstFree(tree, table);
        plWordTableFree(table);
    }

    if (argc > 1) {
        fclose(f);
    }

    return ret;
}
