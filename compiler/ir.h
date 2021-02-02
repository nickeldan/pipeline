#ifndef PIPELINE_COMPILER_IR_H
#define PIPELINE_COMPILER_IR_H

#include "ast.h"
#include "definitions.h"

typedef struct plIr plIr;

int
plIrGenerate(const char *file_name, const plAstNode *tree, plIr **ir);

void
plIrFree(plIr *ir);

#endif  // PIPELINE_COMPILER_IR_H
