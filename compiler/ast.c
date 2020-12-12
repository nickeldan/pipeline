#include <stdlib.h>
#include <string.h>

#include "vasq/logger.h"

#include "ast.h"

plAstNode *
plAstNew(int node_type)
{
    plAstNode *node;

    switch (plAstSplitSize(node_type)) {
    case 0:
        node = VASQ_MALLOC(sizeof(plAstNode));
        if (node) {
            *node = (plAstNode){0};
            goto set_node_type;
        }
        break;

    case 1:
        node = VASQ_MALLOC(sizeof(plAstOneSplitNode));
        if (node) {
            *(plAstOneSplitNode *)node = (plAstOneSplitNode){0};
            goto set_node_type;
        }
        break;

    case 2:
        node = VASQ_MALLOC(sizeof(plAstTwoSplitNode));
        if (node) {
            *(plAstTwoSplitNode *)node = (plAstTwoSplitNode){0};
            goto set_node_type;
        }
        break;

    case 3:
        node = VASQ_MALLOC(sizeof(plAstThreeSplitNode));
        if (node) {
            *(plAstThreeSplitNode *)node = (plAstThreeSplitNode){0};
            goto set_node_type;
        }
        break;

    case 4:
        node = VASQ_MALLOC(sizeof(plAstFourSplitNode));
        if (node) {
            *(plAstFourSplitNode *)node = (plAstFourSplitNode){0};
            goto set_node_type;
        }
        break;

    default: VASQ_ERROR("Invalid node type: %i", node_type); break;
    }

    return NULL;

set_node_type:

    node->token.marker = node_type;
    return node;
}

void
plAstFree(plAstNode *node, plNameTable *table)
{
    int split_size;
    plAstMaxSplitNode *splitter;

    if (!node) {
        return;
    }

    split_size = plAstSplitSize(node->token.marker);
    splitter = (plAstMaxSplitNode *)node;
    for (int k = 0; k < split_size; k++) {
        plAstFree(splitter->nodes[k], table);
    }

    plTokenCleanup(&node->token, table);
    free(node);
}

int
plAstSplitSize(int node_type)
{
    switch (node_type) {
    case PL_LMARKER_NAME:
    case PL_LMARKER_LITERAL:
    case PL_LMARKER_TYPE:
    case PL_LMARKER_IMPORT:
    case PL_LMARKER_EXPORT: return 0;

    case PL_LMARKER_SEMICOLON:
    case PL_LMARKER_COLON:
    case PL_LMARKER_PERIOD:
    case PL_LMARKER_COMMA:
    case PL_LMARKER_LOCAL: return 2;

    case PL_LMARKER_SINK: return 3;

    case PL_LMARKER_SOURCE:
    case PL_LMARKER_PIPE: return 4;

    default: return -1;
    }
}
