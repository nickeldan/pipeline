#ifndef PIPELINE_COMPILER_GENERATE_H
#define PIPELINE_COMPILER_GENERATE_H

// util header files
#include "util.h"

// vm header files
#include "module.h"

// compiler/parser header files
#include "ast.h"

int
plGenerateModule(plAstNode *tree, const char *file_name, plModule *module, uint32_t compiler_flags);

#define PL_COMPILER_FLAG_EXPORT_ALL 0x80000000

#endif  // PIPELINE_COMPILER_GENERATE_H