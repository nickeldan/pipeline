#ifndef PIPELINE_COMPILER_GENERATE_H
#define PIPELINE_COMPILER_GENERATE_H

#include "module.h"

#include "ast.h"
#include "definitions.h"

int
plGenerateModule(plAstNode *tree, const char *file_name, plModule *module, uint32_t compiler_flags);

#endif  // PIPELINE_COMPILER_GENERATE_H