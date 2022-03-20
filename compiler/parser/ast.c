#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "scanner.h"

typedef struct plAstNodeWithData {
    plLexicalTokenHeader header;
    plLexicalTokenData data;
} plAstNodeWithData;

typedef struct plAstOneSplitNode {
    plLexicalTokenHeader header;
    plAstNode *nodes[1];
} plAstOneSplitNode;

typedef struct plAstTwoSplitNode {
    plLexicalTokenHeader header;
    plAstNode *nodes[2];
} plAstTwoSplitNode;

typedef struct plAstThreeSplitNode {
    plLexicalTokenHeader header;
    plAstNode *nodes[3];
} plAstThreeSplitNode;

typedef struct plAstFourSplitNode {
    plLexicalTokenHeader header;
    plAstNode *nodes[4];
} plAstFourSplitNode;

typedef plAstFourSplitNode plAstSplitter;

plAstNode *
plAstGetChild(const plAstNode *parent, unsigned int which)
{
    int node_type, split_size __attribute__((unused));
    const plAstSplitter *splitter = (const plAstSplitter *)parent;

    if (UNLIKELY(!parent)) {
        VASQ_ERROR(debug_logger, "parent cannot be NULL.");
        return NULL;
    }

    node_type = parent->header.marker;
    split_size = plAstSplitSize(node_type);
    if (UNLIKELY(split_size <= 0)) {
        VASQ_ERROR(debug_logger, "This node type (%s) does not have any children.",
                   plLexicalMarkerName(node_type));
        return NULL;
    }

    if (UNLIKELY(which >= (unsigned int)split_size)) {
        VASQ_ERROR(debug_logger, "%u is too high an index for this node type (%s).", which,
                   plLexicalMarkerName(node_type));
        return NULL;
    }

    return splitter->nodes[which];
}

bool
plAstSetChild(plAstNode *parent, unsigned int which, plAstNode *child)
{
    int node_type, split_size __attribute__((unused));
    plAstSplitter *splitter = (plAstSplitter *)parent;

    if (UNLIKELY(!parent)) {
        VASQ_ERROR(debug_logger, "parent cannot be NULL.");
        return false;
    }

    node_type = parent->header.marker;
    split_size = plAstSplitSize(node_type);
    if (UNLIKELY(split_size <= 0)) {
        VASQ_ERROR(debug_logger, "This node type (%s) does not have any children.",
                   plLexicalMarkerName(node_type));
        return false;
    }

    if (UNLIKELY(which >= (unsigned int)split_size)) {
        VASQ_ERROR(debug_logger, "%u is too high an index for this node type (%s).", which,
                   plLexicalMarkerName(node_type));
        return false;
    }

    splitter->nodes[which] = child;
    return true;
}

plLexicalTokenData *
plAstGetData(plAstNode *node)
{
    int node_type;
    plAstNodeWithData *node_with_data = (plAstNodeWithData *)node;

    if (UNLIKELY(!node)) {
        VASQ_ERROR(debug_logger, "node cannot be NULL.");
        return NULL;
    }

    node_type = node->header.marker;
    if (UNLIKELY(plAstSplitSize(node_type) != -1)) {
        VASQ_ERROR(debug_logger, "This node type (%s) does not have data.", plLexicalMarkerName(node_type));
        return NULL;
    }

    return &node_with_data->data;
}

plAstNode *
plAstNew(int node_type)
{
    plAstNode *node;

    switch (plAstSplitSize(node_type)) {
    case -1:
        node = plSafeMalloc(sizeof(plAstNodeWithData));
        if (node) {
            *(plAstNodeWithData *)node = (plAstNodeWithData){0};
            goto set_node_type;
        }
        break;

    case 0:
        node = plSafeMalloc(sizeof(plAstNode));
        if (node) {
            *node = (plAstNode){0};
            goto set_node_type;
        }
        break;

    case 1:
        node = plSafeMalloc(sizeof(plAstOneSplitNode));
        if (node) {
            *(plAstOneSplitNode *)node = (plAstOneSplitNode){0};
            goto set_node_type;
        }
        break;

    case 2:
        node = plSafeMalloc(sizeof(plAstTwoSplitNode));
        if (node) {
            *(plAstTwoSplitNode *)node = (plAstTwoSplitNode){0};
            goto set_node_type;
        }
        break;

    case 3:
        node = plSafeMalloc(sizeof(plAstThreeSplitNode));
        if (node) {
            *(plAstThreeSplitNode *)node = (plAstThreeSplitNode){0};
            goto set_node_type;
        }
        break;

    case 4:
        node = plSafeMalloc(sizeof(plAstFourSplitNode));
        if (node) {
            *(plAstFourSplitNode *)node = (plAstFourSplitNode){0};
            goto set_node_type;
        }
        break;

    default:
        if (VALID_MARKER(node_type)) {
            VASQ_ERROR(debug_logger, "Invalid node type: %s", plLexicalMarkerName(node_type));
        }
        else {
            VASQ_ERROR(debug_logger, "Invalid node type: %i", node_type);
        }
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

    if (!node) {
        return;
    }

    split_size = plAstSplitSize(node->header.marker);
    if (split_size == -1) {
        plLexicalToken token;

        memcpy(&token.header, &node->header, sizeof(node->header));
        memcpy(&token.data, &((plAstNodeWithData *)node)->data, sizeof(token.data));
        plTokenCleanup(&token, table);
    }
    else {
        plAstSplitter *splitter = (plAstSplitter *)node;

        for (int k = 0; k < split_size; k++) {
            plAstFree(splitter->nodes[k], table);
        }
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
    case PL_MARKER_EXIT:
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
    plAstSplitter *splitter;

    parent = plAstNew(marker);
    if (UNLIKELY(!parent)) {
        return NULL;
    }
    splitter = (plAstSplitter *)parent;

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

    if (UNLIKELY(!first || !second)) {
        VASQ_ERROR(debug_logger, "first and second cannot be NULL.");
        return PL_RET_USAGE;
    }

    if (UNLIKELY(!*first)) {
        VASQ_ERROR(debug_logger, "*first cannot be NULL.");
        return PL_RET_USAGE;
    }

    if (UNLIKELY(plAstSplitSize(marker) != 2)) {
        VASQ_ERROR(debug_logger, "The marker must have a split size of 2.");
        return PL_RET_USAGE;
    }

    parent = plAstCreateFamily(marker, *first, second);
    *first = parent;

    return PL_RET_OK;
}

void
plAstPrint(const plAstNode *node, unsigned int margin)
{
    int split_size;
    const plAstSplitter *splitter = (const plAstSplitter *)node;

    for (unsigned int k = 0; k < margin; k++) {
        printf("\t");
    }

    if (!node) {
        printf("(null)\n");
        return;
    }

    printf("%s", plLexicalMarkerName(node->header.marker));
    if (node->header.marker == PL_MARKER_NAME) {
        const plAstNodeWithData *node_with_data = (const plAstNodeWithData *)node;
        printf(" (%s)", node_with_data->data.name);
    }
    printf("\n");

    split_size = plAstSplitSize(node->header.marker);
    for (int k = 0; k < split_size; k++) {
        plAstPrint(splitter->nodes[k], margin + 1);
    }
}
