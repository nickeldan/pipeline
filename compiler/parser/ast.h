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

#define AST_CHILD(node, which) (((plAstSplitter *)(node))->nodes[which])
#define AST_DATA(node)         (&((plAstNodeWithData *)(node))->data)

plLexicalTokenData *
plAstGetData(plAstNode *node);

bool
plAstSetName(plAstNode *node, const char *name);

bool
plAstSetHandle(plAstNode *node, const plObjectHandle *handle);

plAstNode *
plAstNew(int marker, const plLexicalToken *token);

void
plAstFree(plAstNode *node, plWordTable *table);

int
plAstSplitSize(int node_type) __attribute__((pure));

plAstNode *
plAstCreateFamily(int marker, const plLexicalToken *token, ...);

void
plAstCreateConnection(int marker, const plLexicalToken *token, plAstNode **first, plAstNode *second);

void
plAstPrint(const plAstNode *node, unsigned int margin);

#endif  // PIPELINE_COMPILER_AST_H
