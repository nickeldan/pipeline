#include "parserInternal.h"

int
parseWhileBlock(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    unsigned int line_no;
    plAstNode *condition_node, *statement_list;

    line_no = plLastLineNo(scanner);
    *node = NULL;

    ret = parseExpression(scanner, &condition_node, false);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = expectMarker(scanner, PL_MARKER_LEFT_BRACE, NULL);
    if (ret != PL_RET_OK) {
        plAstFree(condition_node, scanner->table);
        return ret;
    }

    ret = parseStatementList(scanner, &statement_list);
    if (ret != PL_RET_OK) {
        plAstFree(condition_node, scanner->table);
        return ret;
    }

    *node = plAstNew(PL_MARKER_WHILE);
    if (!*node) {
        plAstFree(condition_node, scanner->table);
        plAstFree(statement_list, scanner->table);
        return PL_RET_OUT_OF_MEMORY;
    }
    (*node)->token.line_no = line_no;
    createFamily(*node, condition_node, statement_list);

    return PL_RET_OK;
}
