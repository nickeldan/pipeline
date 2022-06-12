#ifndef PIPELINE_COMPILER_SCANNER_H
#define PIPELINE_COMPILER_SCANNER_H

#include <stdio.h>

// util header files
#include "table.h"
#include "util.h"

#include "token.h"

#define PL_SCANNER_BUFFER_SIZE 200
#define PL_SCANNER_MAX_STORE   2

typedef struct plLexicalScanner {
    vasqLogger *scanner_logger;
    vasqLogger *parser_logger;
    FILE *file;
    plWordTable *table;
    plRefTable *keyword_table;
    plLineTable *line_table;
    const char *file_name;
    char *line;
    plLexicalToken store[PL_SCANNER_MAX_STORE];
    plLexicalLocation location;
    plLexicalLocation last_consumed_location;
    unsigned int line_length;
    unsigned int whitespace_stripped;
    unsigned char num_stored;
    char error_on_peek;
    char buffer[PL_SCANNER_BUFFER_SIZE];
    unsigned int inside_comment_block : 1;
} plLexicalScanner;

#define PARSER_ERROR(format, ...) VASQ_ERROR(scanner->parser_logger, format, ##__VA_ARGS__)

int
plScannerInit(plLexicalScanner *scanner, FILE *file, const char *file_name);

void
plScannerCleanup(plLexicalScanner *scanner);

#define PEEK_TOKEN(scanner, spot) ((scanner)->store[spot].header.marker)

int
plTokenConsume(plLexicalScanner *scanner, plLexicalToken *token);

int
plTranslateTerminalMarker(int marker);

#if LL_USE == -1

#define CONSUME_TOKEN(scanner, token) plTokenConsume(scanner, token)

#else

int
plTokenConsumeLog(const char *file_name, const char *function_name, unsigned int line_no,
                  plLexicalScanner *scanner, plLexicalToken *token);
#define CONSUME_TOKEN(scanner, token) plTokenConsumeLog(__FILE__, __func__, __LINE__, scanner, token)

#endif  // LL_USE == -1

void
plTokenCleanup(plLexicalToken *token, plWordTable *table);

#endif  // PIPELINE_COMPILER_SCANNER_H
