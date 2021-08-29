#ifndef PIPELINE_COMPILER_PARSER_INTERNAL_H
#define PIPELINE_COMPILER_PARSER_INTERNAL_H

// util header files
#include "util.h"

#include "ast.h"
#include "scanner.h"

#if LL_USE == -1

int
plNextTokenNoLog(plLexicalScanner *scanner, plLexicalToken *token);
#define NEXT_TOKEN(scanner, token) plNextTokenNoLog(scanner, token)

int
plExpectMarkerNoLog(plLexicalScanner *scanner, int marker, plLexicalLocation *location);
#define EXPECT_MARKER(scanner, marker, location) plExpectMarkerNoLog(scanner, marker, location)

#else  // LL_USE == -1

int
plNextTokenLog(const char *file_name, const char *function_name, unsigned int line_no,
               plLexicalScanner *scanner, plLexicalToken *token);
#define NEXT_TOKEN(scanner, token) plNextTokenLog(__FILE__, __func__, __LINE__, scanner, token)

int
plExpectMarkerLog(const char *file_name, const char *function_name, unsigned int line_no,
                  plLexicalScanner *scanner, int marker, plLexicalLocation *location);
#define EXPECT_MARKER(scanner, marker, location) \
    plExpectMarkerLog(__FILE__, __func__, __LINE__, scanner, marker, location)

#endif  // LL_USE == -1

int
plParseExtendedName(plLexicalScanner *scanner, plAstNode **node);

int
plParseExtendedType(plLexicalScanner *scanner, plAstNode **node);

int
plParseArrayDeclaration(plLexicalScanner *scanner, plAstNode **node, bool compilation_only);

int
plParseStructDefinition(plLexicalScanner *scanner, plAstNode **node);

int
plParseExpression(plLexicalScanner *scanner, plAstNode **node, bool compilation_only);

int
plParseReceiver(plLexicalScanner *scanner, plAstNode **node);

int
plParseIfBlock(plLexicalScanner *scanner, plAstNode **node);

int
plParseWhileBlock(plLexicalScanner *scanner, plAstNode **node);

int
plParseStatementList(plLexicalScanner *scanner, plAstNode **node);

int
plParseFunction(plLexicalScanner *scanner, plAstNode **node, bool global);

#endif  // PIPELINE_COMPILER_PARSER_INTERNAL_H
