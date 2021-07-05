#ifndef PIPELINE_COMPILER_PARSER_H
#define PIPELINE_COMPILER_PARSER_H

#include <stdio.h>

#include "ast.h"
#include "definitions.h"
#include "table.h"

int
plFileParse(FILE *in, const char *file_name, plAstNode **tree, plWordTable **table);

#endif  // PIPELINE_COMPILER_PARSER_H
