#ifndef PIPELINE_COMPILER_PARSER_INTERNAL_H
#define PIPELINE_COMPILER_PARSER_INTERNAL_H

// util header files
#include "util.h"

#include "ast.h"
#include "scanner.h"

#if LL_USE == -1

int HIDDEN_SYMBOL
plExpectMarker(plLexicalScanner *scanner, int marker, plLexicalToken *token);
#define EXPECT_MARKER(scanner, marker, token) plExpectMarker(scanner, marker, token)

#else

int HIDDEN_SYMBOL
plExpectMarker(const char *file_name, const char *function_name, unsigned int line_no,
               plLexicalScanner *scanner, int marker, plLexicalToken *token);
#define EXPECT_MARKER(scanner, marker, token) \
    plExpectMarker(__FILE__, __func__, __LINE__, scanner, marker, token)

#endif  // LL_USE == -1

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
