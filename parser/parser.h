#ifndef __PIPELINE_PARSER_H__
#define __PIPELINE_PARSER_H__

#include "error.h"
#include "module.h"

plErrorCode parse_source_code(const char *path, plModule_t *module);

#endif // __PIPELINE_PARSER_H__