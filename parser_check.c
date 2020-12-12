#include <unistd.h>

#include "vasq/logger.h"

#include "parser.h"

int main(int argc, char **argv) {
    int ret;
    FILE *f;
    plNameTable *table;
    plAstNode *tree;

    if ( VASQ_LOG_INIT(LL_USE, STDERR_FILENO, false) != VASQ_RET_OK ) {
        return 1;
    }

    if ( argc == 1 ) {
        f = stdin;
    }
    else {
        f = fopen(argv[1], "r");
        if ( !f ) {
            VASQ_PERROR("fopen", errno);
            return 2;
        }
    }

    ret = plFileParse(f, argv[1], &tree, &table);
    if ( ret == PL_RET_OK ) {
        plAstFree(tree, table);
        plNameTableFree(table);
    }
    else {
        ret = 4;
    }

    if ( argc > 1 ) {
        fclose(f);
    }

    return ret;
}
