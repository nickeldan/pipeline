#ifndef __PIPELINE_PARSER_WRAPPER_H__
#define __PIPELINE_PARSER_WRAPPER_H__

typedef void *yyscan_t;

#define SETUP_SCANNER struct yyguts_t *yyg __attribute__((unused)) = (struct yyguts_t *)yyscanner

#include "parser.tab.h"

#endif  // __PIPELINE_PARSER_WRAPPER_H__
