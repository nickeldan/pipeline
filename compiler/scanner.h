#ifndef PIPELINE_COMPILER_SCANNER_H
#define PIPELINE_COMPILER_SCANNER_H

#include <ctype.h>
#include <stdio.h>

#include "table.h"
#include "token.h"

#define PL_SCANNER_MAX_LOOK_AHEAD 2

typedef struct plLexicalScanner {
    plLexicalToken look_ahead[PL_SCANNER_MAX_LOOK_AHEAD];
    FILE *file;
    plWordTable *table;
    const char *file_name;
    char *line;
    plLexicalLocation location;
    plLexicalLocation last_look_ahead_loc;
    unsigned int comment_block_line_no;
    unsigned int line_length;
    int last_marker;
    bool inside_comment_block;
    unsigned char num_look_ahead;
    char buffer[200];
} plLexicalScanner;

void
plScannerInit(plLexicalScanner *scanner, FILE *file, const char *file_name, plWordTable *table);

void
plScannerCleanup(plLexicalScanner *scanner);

int
plTokenRead(plLexicalScanner *scanner, plLexicalToken *token);

int
plTranslateTerminalMarker(int marker);

#if LL_USE == -1

#define TOKEN_READ(scanner, token) plTokenRead(scanner, token)

int
plLookaheadStoreNoLog(plLexicalScanner *scanner, plLexicalToken *token);
#define LOOKAHEAD_STORE(scanner, token) plLookaheadStoreNoLog(scanner, token)

#else

int
plTokenReadLog(const char *file_name, const char *function_name, unsigned int line_no,
               plLexicalScanner *scanner, plLexicalToken *token);
#define TOKEN_READ(scanner, token)      plTokenReadLog(__FILE__, __func__, __LINE__, scanner, token)

int
plLookaheadStoreLog(const char *file_name, const char *function_name, unsigned int line_no,
                    plLexicalScanner *scanner, plLexicalToken *token);
#define LOOKAHEAD_STORE(scanner, token) plLookaheadStoreLog(__FILE__, __func__, __LINE__, scanner, token)

#endif  // LL_USE == -1

void
plGetLastLocation(const plLexicalScanner *scanner, plLexicalLocation *location);

void
plTokenCleanup(plLexicalToken *token, plWordTable *table);

#if LL_USE == -1

void
parserErrorNoLog(const plLexicalScanner *scanner, const char *format, ...);
#define PARSER_ERROR(format, ...) parserErrorNoLog(scanner, format, ##__VA_ARGS__)

#else

void
parserErrorLog(const char *file_name, const char *function_name, unsigned int line_no,
               const plLexicalScanner *scanner, const char *format, ...);
#define PARSER_ERROR(format, ...) \
    parserErrorLog(__FILE__, __func__, __LINE__, scanner, format, ##__VA_ARGS__)

#endif  // LL_USE == -1

#endif  // PIPELINE_COMPILER_SCANNER_H
