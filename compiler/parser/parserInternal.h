#ifndef PIPELINE_COMPILER_PARSER_INTERNAL_H
#define PIPELINE_COMPILER_PARSER_INTERNAL_H

#include "ast.h"
#include "definitions.h"
#include "scanner.h"

#if LL_USE == -1

int
nextTokenNoLog(plLexicalScanner *scanner, plLexicalToken *token);
#define NEXT_TOKEN(scanner, token) nextTokenNoLog(scanner, token)

int
expectMarkerNoLog(plLexicalScanner *scanner, int marker, plLexicalLocation *location);
#define EXPECT_MARKER(scanner, marker, location) expectMarkerNoLog(scanner, marker, location)

#else  // LL_USE == -1

int
nextTokenLog(const char *file_name, const char *function_name, unsigned int line_no,
             plLexicalScanner *scanner, plLexicalToken *token);
#define NEXT_TOKEN(scanner, token) nextTokenLog(__FILE__, __func__, __LINE__, scanner, token)

int
expectMarkerLog(const char *file_name, const char *function_name, unsigned int line_no,
                plLexicalScanner *scanner, int marker, plLexicalLocation *location);
#define EXPECT_MARKER(scanner, marker, location) \
    expectMarkerLog(__FILE__, __func__, __LINE__, scanner, marker, location)

#endif  // LL_USE == -1

int
parseExtendedName(plLexicalScanner *scanner, plAstNode **node);

int
parseExtendedType(plLexicalScanner *scanner, plAstNode **node);

int
parseArrayDeclaration(plLexicalScanner *scanner, plAstNode **node, bool compilation_only);

int
parseStructDefinition(plLexicalScanner *scanner, plAstNode **node);

int
parseExpression(plLexicalScanner *scanner, plAstNode **node, bool compilation_only);

int
parseReceiver(plLexicalScanner *scanner, plAstNode **node);

int
parseIfBlock(plLexicalScanner *scanner, plAstNode **node);

int
parseWhileBlock(plLexicalScanner *scanner, plAstNode **node);

int
parseStatementList(plLexicalScanner *scanner, plAstNode **node);

int
parseFunction(plLexicalScanner *scanner, plAstNode **node, bool global);

#endif  // PIPELINE_COMPILER_PARSER_INTERNAL_H
