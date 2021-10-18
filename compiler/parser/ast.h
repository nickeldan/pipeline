#ifndef PIPELINE_COMPILER_AST_H
#define PIPELINE_COMPILER_AST_H

// util header files
#include "table.h"
#include "util.h"

#include "token.h"

#define AST_HEADER plLexicalTokenHeader header;

typedef struct plAstNode {
    AST_HEADER
} plAstNode;

typedef struct plAstNodeWithData {
    AST_HEADER
    plLexicalTokenData data;
} plAstNodeWithData;

#define NODE_EXTRACT_NAME(node)   (((plAstNodeWithData *)(node))->data.name)
#define NODE_EXTRACT_HANDLE(node) (((plAstNodeWithData *)(node))->data.handle)

typedef struct plAstOneSplitNode {
    AST_HEADER
    plAstNode *nodes[1];
} plAstOneSplitNode;

typedef struct plAstTwoSplitNode {
    AST_HEADER
    plAstNode *nodes[2];
} plAstTwoSplitNode;

typedef struct plAstThreeSplitNode {
    AST_HEADER
    plAstNode *nodes[3];
} plAstThreeSplitNode;

typedef struct plAstFourSplitNode {
    AST_HEADER
    plAstNode *nodes[4];
} plAstFourSplitNode;

typedef plAstFourSplitNode plAstMaxSplitNode;

plAstNode *
plAstNew(int node_type);

void
plAstFree(plAstNode *node, plWordTable *table);

int
plAstSplitSize(int node_type) __attribute__((pure));

plAstNode *
plAstCreateFamily(int marker, ...);

int
plAstCreateConnection(int marker, plAstNode **first, plAstNode *second);

void
plAstPrint(const plAstNode *node, unsigned int margin);

#endif  // PIPELINE_COMPILER_AST_H
