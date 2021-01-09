#include <unistd.h>

#include "vasq/logger.h"

#include "parser.h"
#include "plUtil.h"

void
printAst(const plAstNode *node, unsigned int margin);

int
main(int argc, char **argv)
{
    int ret;
    FILE *f;
    plNameTable *table;
    plAstNode *tree;

    ret = VASQ_LOG_INIT(LL_USE, STDERR_FILENO, false);
    if (ret != VASQ_RET_OK) {
        return plTranslateVasqRet(ret);
    }

    if (argc == 1) {
        f = stdin;
    }
    else {
        f = fopen(argv[1], "r");
        if (!f) {
            VASQ_PERROR("fopen", errno);
            return PL_RET_NO_ACCESS;
        }
    }

    ret = plFileParse(f, argv[1], &tree, &table);
    if (ret == PL_RET_OK) {
        printAst(tree, 0);

        plAstFree(tree, table);
        plNameTableFree(table);
    }

    if (argc > 1) {
        fclose(f);
    }

    return ret;
}

void
printAst(const plAstNode *node, unsigned int margin)
{
    int split_size;
    const plAstMaxSplitNode *splitter = (const plAstMaxSplitNode *)node;

    if ( !node ) {
        return;
    }

    for (unsigned int k=0; k<margin; k++) {
        printf("\t");
    }
    printf("%s", plLexicalMarkerName(node->token.marker));
    if ( node->token.marker == PL_MARKER_NAME ) {
        printf(" (%s)", node->token.ctx.name);
    }
    printf("\n");

    split_size = plAstSplitSize(node->token.marker);
    for (int k=0; k<split_size; k++) {
        printAst(splitter->nodes[k], margin+1);
    }
}
