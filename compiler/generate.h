#ifndef PIPELINE_COMPILER_GENERATE_H
#define PIPELINE_COMPILER_GENERATE_H

#include "module.h"

#include "ast.h"
#include "definitions.h"

int
plCompileModule(plAstNode *tree, plModule **module, uint32_t compiler_flags);

#endif  // PIPELINE_COMPILER_GENERATE_H