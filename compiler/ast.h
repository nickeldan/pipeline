#pragma once

#define AST_HAS_PARENT

#ifdef AST_HAS_PARENT
#define AST_PARENT_DECL struct plAstNode *parent;
#else
#define AST_PARENT_DECL
#endif

#define AST_HEADER \
    AST_PARENT_DECL \
    void *ctx; \
    int node_type; \
    unsigned int line_no;

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
plAstNew(int node_type, unsigned int line_no, void *ctx);

void
plAstFree(astNode *node);

unsigned int
plAstSplitSize(int node_type) __attribute__ ((pure));
