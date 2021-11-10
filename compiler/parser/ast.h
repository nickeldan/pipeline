#ifndef PIPELINE_COMPILER_AST_H
#define PIPELINE_COMPILER_AST_H

// util header files
#include "table.h"
#include "util.h"

#include "token.h"

typedef struct plAstNode {
    plLexicalTokenHeader header;
} plAstNode;

typedef struct plAstNodeWithData {
    plLexicalTokenHeader header;
    plLexicalTokenData data;
} plAstNodeWithData;

#define NODE_EXTRACT_NAME(node)   (((plAstNodeWithData *)(node))->data.name)
#define NODE_EXTRACT_HANDLE(node) (((plAstNodeWithData *)(node))->data.handle)

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
