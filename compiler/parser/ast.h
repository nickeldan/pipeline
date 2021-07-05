#ifndef PIPELINE_COMPILER_AST_H
#define PIPELINE_COMPILER_AST_H

#include "definitions.h"
#include "scanner.h"
#include "table.h"

//#define AST_HAS_PARENT

#ifdef AST_HAS_PARENT
#define AST_PARENT_DECL struct plAstNode *parent;
#else
#define AST_PARENT_DECL
#endif

#define AST_HEADER  \
    AST_PARENT_DECL \
    plLexicalToken token;

typedef struct plAstNode {
    AST_HEADER
} plAstNode;

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
plAstSplitSize(int node_type);

plAstNode *
plAstCreateFamily(int marker, ...);

int
plAstCreateConnection(int marker, plAstNode **first, plAstNode *second);

void
plAstPrint(const plAstNode *node, unsigned int margin);

#endif  // PIPELINE_COMPILER_AST_H
