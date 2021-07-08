#ifndef PIPELINE_COMPILER_GENERATE_H
#define PIPELINE_COMPILER_GENERATE_H

#include "module.h"

#include "ast.h"
#include "definitions.h"

int
plGenerateModule(plAstNode *tree, const char *file_name, plModule *module, uint32_t compiler_flags);

#define PL_COMPILER_FLAG_EXPORT_ALL 0x80000000

#endif  // PIPELINE_COMPILER_GENERATE_H