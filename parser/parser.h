#ifndef __PIPELINE_PARSER_H__
#define __PIPELINE_PARSER_H__

#include "error.h"
#include "module.h"

plErrorCode parseSourceCode(const char *path, plModule *module);

#endif // __PIPELINE_PARSER_H__