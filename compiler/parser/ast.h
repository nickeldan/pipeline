#ifndef PIPELINE_COMPILER_AST_H
#define PIPELINE_COMPILER_AST_H

// util header files
#include "table.h"
#include "util.h"

#include "token.h"

typedef struct plAstNode {
    plLexicalTokenHeader header;
} plAstNode;

plAstNode *
plAstGetChild(const plAstNode *parent, unsigned int which);

bool
plAstSetChild(plAstNode *parent, unsigned int which, plAstNode *child);

plLexicalTokenData *
plAstGetData(plAstNode *node);

bool
plAstSetName(plAstNode *node, const char *name);

bool
plAstSetHandle(plAstNode *node, const plObjectHandle *handle);

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
