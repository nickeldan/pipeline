#include <unistd.h>

#include "vasq/logger.h"

#include "parser.h"
#include "plUtil.h"

int
main(int argc, char **argv)
{
    int ret;
    FILE *f;
    const char *file_name;
    plNameTable *table;
    plAstNode *tree;

    ret = VASQ_LOG_INIT(LL_USE, STDOUT_FILENO, false);
    if (ret != VASQ_RET_OK) {
        return plTranslateVasqRet(ret);
    }

    if (argc == 1) {
        f = stdin;
        file_name = "stdin";
    }
    else {
        f = fopen(argv[1], "r");
        if (!f) {
            VASQ_PERROR("fopen", errno);
            return PL_RET_NO_ACCESS;
        }
        file_name = argv[1];
    }

    ret = plFileParse(f, file_name, &tree, &table);
    if (ret == PL_RET_OK) {
        plAstPrint(tree, 0);
        plAstFree(tree, table);
        plNameTableFree(table);
    }

    if (argc > 1) {
        fclose(f);
    }

    return ret;
}
