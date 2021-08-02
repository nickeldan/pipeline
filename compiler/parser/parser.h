#ifndef PIPELINE_COMPILER_PARSER_H
#define PIPELINE_COMPILER_PARSER_H

#include <stdio.h>

// util header files
#include "table.h"

// compiler/util header files
#include "definitions.h"

#include "ast.h"

int
plFileParse(FILE *in, const char *file_name, plAstNode **tree, plWordTable **table);

#endif  // PIPELINE_COMPILER_PARSER_H
