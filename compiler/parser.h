#pragma once

#include <stdio.h>

#include "ast.h"
#include "definitions.h"
#include "nameTable.h"

int
plFileParse(FILE *in, const char *file_name, plAstNode **tree, plNameTable **table);
