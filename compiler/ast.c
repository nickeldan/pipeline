#include <stdlib.h>
#include <string.h>

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
    case PL_MARKER_NAME:
    case PL_MARKER_OBJECT:
    case PL_MARKER_DROP:
    case PL_MARKER_END:
    case PL_MARKER_BREAK:
    case PL_MARKER_CONT:
    case PL_MARKER_TYPE:
    case PL_MARKER_IMPORT:
    case PL_MARKER_EXPORT: return 0;

    case PL_MARKER_LEFT_BRACKET:  // For array declarations.
    case PL_MARKER_PROD:
    case PL_MARKER_ELSE:
    case PL_MARKER_VERIFY:
    case PL_MARKER_ABORT:
    case PL_MARKER_MAIN:
    case '%': return 1;

    case PL_MARKER_SEMICOLON:
    case PL_MARKER_COLON:
    case PL_MARKER_PERIOD:
    case PL_MARKER_COMMA:
    case PL_MARKER_WHILE:
    case PL_MARKER_IS:
    case PL_MARKER_AS:
    case PL_MARKER_LOCAL:
    case 'A':
    case 'C': return 2;

    case PL_MARKER_EIF:
    case PL_MARKER_SINK: return 3;

    case PL_MARKER_IF:
    case PL_MARKER_SOURCE:
    case PL_MARKER_PIPE: return 4;

    default: return -1;
    }
}
