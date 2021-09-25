#ifndef PIPELINE_COMPILER_PARSER_INTERNAL_H
#define PIPELINE_COMPILER_PARSER_INTERNAL_H

// util header files
#include "util.h"

#include "ast.h"
#include "scanner.h"

#if LL_USE == -1

int HIDDEN_SYMBOL
plNextTokenNoLog(plLexicalScanner *scanner, plLexicalToken *token);
#define NEXT_TOKEN(scanner, token) plNextTokenNoLog(scanner, token)

int HIDDEN_SYMBOL
plExpectMarkerNoLog(plLexicalScanner *scanner, int marker, plLexicalLocation *location);
#define EXPECT_MARKER(scanner, marker, location) plExpectMarkerNoLog(scanner, marker, location)

#else  // LL_USE == -1

int HIDDEN_SYMBOL
plNextTokenLog(const char *file_name, const char *function_name, unsigned int line_no,
               plLexicalScanner *scanner, plLexicalToken *token);
#define NEXT_TOKEN(scanner, token) plNextTokenLog(__FILE__, __func__, __LINE__, scanner, token)

int HIDDEN_SYMBOL
plExpectMarkerLog(const char *file_name, const char *function_name, unsigned int line_no,
                  plLexicalScanner *scanner, int marker, plLexicalLocation *location);
#define EXPECT_MARKER(scanner, marker, location) \
    plExpectMarkerLog(__FILE__, __func__, __LINE__, scanner, marker, location)

#endif  // LL_USE == -1

void HIDDEN_SYMBOL
plAstCopyTokenInfo(plAstNode *node, const plLexicalToken *token);

int HIDDEN_SYMBOL
plParseExtendedName(plLexicalScanner *scanner, plAstNode **node);

int HIDDEN_SYMBOL
plParseExtendedType(plLexicalScanner *scanner, plAstNode **node);

int HIDDEN_SYMBOL
plParseArrayDeclaration(plLexicalScanner *scanner, plAstNode **node, bool compilation_only);

int HIDDEN_SYMBOL
plParseStructDefinition(plLexicalScanner *scanner, plAstNode **node);

int HIDDEN_SYMBOL
plParseExpression(plLexicalScanner *scanner, plAstNode **node, bool compilation_only);

int HIDDEN_SYMBOL
plParseReceiver(plLexicalScanner *scanner, plAstNode **node);

int HIDDEN_SYMBOL
plParseIfBlock(plLexicalScanner *scanner, plAstNode **node);

int HIDDEN_SYMBOL
plParseWhileBlock(plLexicalScanner *scanner, plAstNode **node);

int HIDDEN_SYMBOL
plParseStatementList(plLexicalScanner *scanner, plAstNode **node);

int HIDDEN_SYMBOL
plParseFunction(plLexicalScanner *scanner, plAstNode **node, bool global);

#endif  // PIPELINE_COMPILER_PARSER_INTERNAL_H
