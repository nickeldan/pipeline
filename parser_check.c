#include <unistd.h>

#include "vasq/logger.h"

#include "plUtil.h"
#include "parser.h"

int main(int argc, char **argv) {
    int ret;
    FILE *f;
    plNameTable *table;
    plAstNode *tree;

    ret = VASQ_LOG_INIT(LL_USE, STDERR_FILENO, false);
    if ( ret != VASQ_RET_OK ) {
        return plTranslateVasqRet(ret);
    }

    if ( argc == 1 ) {
        f = stdin;
    }
    else {
        f = fopen(argv[1], "r");
        if ( !f ) {
            VASQ_PERROR("fopen", errno);
            return PL_RET_NO_ACCESS;
        }
    }

    ret = plFileParse(f, argv[1], &tree, &table);
    if ( ret == PL_RET_OK ) {
        plAstFree(tree, table);
        plNameTableFree(table);
    }

    if ( argc > 1 ) {
        fclose(f);
    }

    return ret;
}
