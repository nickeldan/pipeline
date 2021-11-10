#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "scanner.h"

plAstNode *
plAstNew(int node_type)
{
    plAstNode *node;

    switch (plAstSplitSize(node_type)) {
    case -1:
        node = VASQ_MALLOC(debug_logger, sizeof(plAstNodeWithData));
        if (node) {
            *(plAstNodeWithData *)node = (plAstNodeWithData){0};
            goto set_node_type;
        }
        break;

    case 0:
        node = VASQ_MALLOC(debug_logger, sizeof(plAstNode));
        if (node) {
            *node = (plAstNode){0};
            goto set_node_type;
        }
        break;

    case 1:
        node = VASQ_MALLOC(debug_logger, sizeof(plAstOneSplitNode));
        if (node) {
            *(plAstOneSplitNode *)node = (plAstOneSplitNode){0};
            goto set_node_type;
        }
        break;

    case 2:
        node = VASQ_MALLOC(debug_logger, sizeof(plAstTwoSplitNode));
        if (node) {
            *(plAstTwoSplitNode *)node = (plAstTwoSplitNode){0};
            goto set_node_type;
        }
        break;

    case 3:
        node = VASQ_MALLOC(debug_logger, sizeof(plAstThreeSplitNode));
        if (node) {
            *(plAstThreeSplitNode *)node = (plAstThreeSplitNode){0};
            goto set_node_type;
        }
        break;

    case 4:
        node = VASQ_MALLOC(debug_logger, sizeof(plAstFourSplitNode));
        if (node) {
            *(plAstFourSplitNode *)node = (plAstFourSplitNode){0};
            goto set_node_type;
        }
        break;

    default:
#if LL_USE != -1
        if (VALID_MARKER(node_type)) {
            VASQ_ERROR(debug_logger, "Invalid node type: %s", plLexicalMarkerName(node_type));
        }
        else {
            VASQ_ERROR(debug_logger, "Invalid node type: %i", node_type);
        }
#endif
        break;
    }

    return NULL;

set_node_type:

    node->header.marker = node_type;
    return node;
}

void
plAstFree(plAstNode *node, plWordTable *table)
{
    int split_size;
    plAstMaxSplitNode *splitter = (plAstMaxSplitNode *)node;

    if (!node) {
        return;
    }

    split_size = plAstSplitSize(node->header.marker);
    for (int k = 0; k < split_size; k++) {
        plAstFree(splitter->nodes[k], table);
    }

    if (split_size == -1) {
        plLexicalToken token;

        memcpy(&token.header, &node->header, sizeof(node->header));
        memcpy(&token.data, &((plAstNodeWithData *)node)->data, sizeof(token.data));
        plTokenCleanup(&token, table);
    }
    free(node);
}

int
plAstSplitSize(int node_type)
{
    switch (node_type) {
    case PL_MARKER_NAME:
    case PL_MARKER_OBJECT: return -1;

    case PL_MARKER_DROP:
    case PL_MARKER_END:
    case PL_MARKER_BREAK:
    case PL_MARKER_CONT:
    case PL_MARKER_TYPE:
    case PL_MARKER_CONTEXT:
    case PL_MARKER_UNDERSCORE:
    case PL_MARKER_EXPORT_ALL: return 0;

    case PL_MARKER_PROD:
    case PL_MARKER_VERIFY:
    case PL_MARKER_ABORT:
    case PL_MARKER_NOT:
    case PL_MARKER_QUESTION:
    case PL_MARKER_IMPORT:
    case PL_MARKER_EXPORT:
    case PL_MARKER_OPAQUE:
    case PL_MARKER_MAIN:
    case 'A':            // For array declarations.
    case '%': return 1;  // For library references.

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
    case PL_MARKER_STRUCT:
    case PL_MARKER_LOCAL:
    case PL_MARKER_LEFT_BRACKET:  // For array indexing.
    case PL_MARKER_LEFT_PARENS:
    case PL_MARKER_REASSIGNMENT: return 2;

    case PL_MARKER_IF:
    case PL_MARKER_SINK: return 3;

    case PL_MARKER_SOURCE:
    case PL_MARKER_PIPE: return 4;

    default: return -2;
    }
}

plAstNode *
plAstCreateFamily(int marker, ...)
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
        splitter->nodes[k] = va_arg(args, plAstNode *);
    }
    va_end(args);

    return parent;
}

int
plAstCreateConnection(int marker, plAstNode **first, plAstNode *second)
{
    plAstNode *parent;

    if (plAstSplitSize(marker) != 2) {
        VASQ_ERROR(debug_logger, "The marker must have a split size of 2.");
        return PL_RET_USAGE;
    }

    if (!first || !second) {
        VASQ_ERROR(debug_logger, "first and second cannot be NULL.");
        return PL_RET_USAGE;
    }

    if (!*first) {
        VASQ_ERROR(debug_logger, "*first cannot be NULL.");
        return PL_RET_USAGE;
    }

    parent = plAstCreateFamily(marker, *first, second);
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

    printf("%s", plLexicalMarkerName(node->header.marker));
    if (node->header.marker == PL_MARKER_NAME) {
        printf(" (%s)", NODE_EXTRACT_NAME(node));
    }
    printf("\n");

    split_size = plAstSplitSize(node->header.marker);
    for (int k = 0; k < split_size; k++) {
        plAstPrint(splitter->nodes[k], margin + 1);
    }
}
