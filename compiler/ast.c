#include <stdio.h>
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

    default:
#if LL_USE != -1
        if (VALID_MARKER(node_type)) {
            VASQ_ERROR("Invalid node type: %s", plLexicalMarkerName(node_type));
        }
        else {
            VASQ_ERROR("Invalid node type: %i", node_type);
        }
#endif
        break;
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
    plAstMaxSplitNode *splitter = (plAstMaxSplitNode *)node;

    if (!node) {
        return;
    }

    split_size = plAstSplitSize(node->token.marker);
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
    case PL_MARKER_CONTEXT:
    case PL_MARKER_UNDERSCORE: return 0;

    case PL_MARKER_PROD:
    case PL_MARKER_VERIFY:
    case PL_MARKER_ABORT:
    case PL_MARKER_NOT:
    case PL_MARKER_QUESTION:
    case PL_MARKER_IMPORT:
    case PL_MARKER_EXPORT:
    case PL_MARKER_MAIN:
    case 'A':  // For array declarations.
    case '%': return 1;

    case PL_MARKER_ARROW:
    case PL_MARKER_ARITHMETIC:
    case PL_MARKER_LOGICAL:
    case PL_MARKER_COMPARISON:
    case PL_MARKER_SEMICOLON:
    case PL_MARKER_COLON:
    case PL_MARKER_PERIOD:
    case PL_MARKER_COMMA:
    case PL_MARKER_WHILE:
    case PL_MARKER_IS:
    case PL_MARKER_AS:
    case PL_MARKER_LOCAL:
    case PL_MARKER_LEFT_BRACKET:  // For array indexing.
    case PL_MARKER_LEFT_PARENS:
    case PL_MARKER_REASSIGNMENT: return 2;

    case PL_MARKER_IF:
    case PL_MARKER_SINK: return 3;

    case PL_MARKER_SOURCE:
    case PL_MARKER_PIPE: return 4;

    default: return -1;
    }
}

plAstNode *
createFamily(int marker, ...)
{
    int split_size;
    va_list args;
    plAstNode *parent;
    plAstMaxSplitNode *splitter;

    parent = plAstNew(marker);
    if (!parent) {
        return NULL;
    }
    splitter = (plAstMaxSplitNode *)parent;

    split_size = plAstSplitSize(marker);
    va_start(args, marker);
    for (int k = 0; k < split_size; k++) {
        plAstNode *node;

        node = va_arg(args, plAstNode *);
#ifdef AST_HAS_PARENT
        if (node) {
            node->parent = parent;
        }
#endif
        splitter->nodes[k] = node;
    }
    va_end(args);

    return parent;
}

int
createConnection(int marker, plAstNode **first, plAstNode *second)
{
    plAstNode *parent;

    if (!first || !second) {
        VASQ_ERROR("The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    if (!*first) {
        VASQ_ERROR("*first cannot be NULL.");
        return PL_RET_USAGE;
    }

    parent = createFamily(marker, *first, second);
    if (!parent) {
        return PL_RET_OUT_OF_MEMORY;
    }
    *first = parent;

    return PL_RET_OK;
}

void
plAstPrint(const plAstNode *node, unsigned int margin)
{
    int split_size;
    const plAstMaxSplitNode *splitter = (const plAstMaxSplitNode *)node;

    for (unsigned int k = 0; k < margin; k++) {
        printf("\t");
    }

    if (!node) {
        printf("(null)\n");
        return;
    }

    printf("%s", plLexicalMarkerName(node->token.marker));
    if (node->token.marker == PL_MARKER_NAME) {
        printf(" (%s)", node->token.ctx.name);
    }
    printf("\n");

    split_size = plAstSplitSize(node->token.marker);
    for (int k = 0; k < split_size; k++) {
        if (splitter->nodes[k]) {
            plAstPrint(splitter->nodes[k], margin + 1);
        }
    }
}
