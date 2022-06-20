#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "scanner.h"

plAstNode *
plAstNew(int marker, const plLexicalToken *token)
{
    plAstNode *node;

    switch (plAstSplitSize(marker)) {
    case -1:
        node = plSafeMalloc(sizeof(plAstNodeWithData));
        if (node) {
            *(plAstNodeWithData *)node = (plAstNodeWithData){0};
            goto set_token;
        }
        break;

    case 0:
        node = plSafeMalloc(sizeof(plAstNode));
        if (node) {
            *node = (plAstNode){0};
            goto set_token;
        }
        break;

    case 1:
        node = plSafeMalloc(sizeof(plAstOneSplitNode));
        if (node) {
            *(plAstOneSplitNode *)node = (plAstOneSplitNode){0};
            goto set_token;
        }
        break;

    case 2:
        node = plSafeMalloc(sizeof(plAstTwoSplitNode));
        if (node) {
            *(plAstTwoSplitNode *)node = (plAstTwoSplitNode){0};
            goto set_token;
        }
        break;

    case 3:
        node = plSafeMalloc(sizeof(plAstThreeSplitNode));
        if (node) {
            *(plAstThreeSplitNode *)node = (plAstThreeSplitNode){0};
            goto set_token;
        }
        break;

    case 4:
        node = plSafeMalloc(sizeof(plAstFourSplitNode));
        if (node) {
            *(plAstFourSplitNode *)node = (plAstFourSplitNode){0};
            goto set_token;
        }
        break;

    default:
        if (VALID_MARKER(marker)) {
            VASQ_ERROR(debug_logger, "Invalid node type: %s", plLexicalMarkerName(marker));
        }
        else {
            VASQ_ERROR(debug_logger, "Invalid node type: %i", marker);
        }
        break;
    }

    return NULL;

set_token:

    if (token) {
        memcpy(&node->header, &token->header, sizeof(token->header));
        if (plAstSplitSize(token->header.marker) == -1) {
            memcpy(AST_DATA(node), &token->data, sizeof(token->data));
        }
    }
    node->header.marker = marker;
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
plAstCreateFamily(int marker, const plLexicalToken *token, ...)
{
    int split_size;
    va_list args;
    plAstNode *parent;

    parent = plAstNew(marker, token);
    if (UNLIKELY(!parent)) {
        return NULL;
    }

    split_size = plAstSplitSize(marker);
    va_start(args, token);
    for (int k = 0; k < split_size; k++) {
        AST_CHILD(parent, k) = va_arg(args, plAstNode *);
    }
    va_end(args);

    return parent;
}

void
plAstCreateConnection(int marker, const plLexicalToken *token, plAstNode **first, plAstNode *second)
{
    if (UNLIKELY(!first || !second)) {
        VASQ_ERROR(debug_logger, "first and second cannot be NULL.");
        exit(PL_RET_USAGE);
    }

    if (UNLIKELY(!*first)) {
        VASQ_ERROR(debug_logger, "*first cannot be NULL.");
        exit(PL_RET_USAGE);
    }

    if (UNLIKELY(plAstSplitSize(marker) != 2)) {
        VASQ_ERROR(debug_logger, "The marker must have a split size of 2.");
        exit(PL_RET_USAGE);
    }

    *first = plAstCreateFamily(marker, token, *first, second);
}

void
plAstPrint(const plAstNode *node, unsigned int margin)
{
    int split_size;

    for (unsigned int k = 0; k < margin; k++) {
        printf("\t");
    }

    if (!node) {
        printf("(null)\n");
        return;
    }

    printf("%s", plLexicalMarkerName(node->header.marker));
    if (node->header.marker == PL_MARKER_NAME) {
        printf(" (%s)", AST_DATA(node)->name);
    }
    printf("\n");

    split_size = plAstSplitSize(node->header.marker);
    for (int k = 0; k < split_size; k++) {
        plAstPrint(AST_CHILD(node, k), margin + 1);
    }
}
