#pragma once

#include "vasq/logger.h"

#include "ast.h"
#include "scanner.h"

#if LL_USE == VASQ_LL_RAWONLY

int
nextTokenNoLog(plLexicalScanner *scanner, plLexicalToken *token);
#define NEXT_TOKEN(scanner, token) nextTokenNoLog(scanner, token)

int
expectMarkerNoLog(plLexicalScanner *scanner, int marker, plLexicalLocation *location);
#define EXPECT_MARKER(scanner, marker, location) expectMarkerNoLog(scanner, marker, location)

#else  // LL_USE == VASQ_LL_RAWONLY

int
nextTokenLog(const char *file_name, const char *function_name, unsigned int line_no,
             plLexicalScanner *scanner, plLexicalToken *token);
#define NEXT_TOKEN(scanner, token) nextTokenLog(__FILE__, __func__, __LINE__, scanner, token)

int
expectMarkerLog(const char *file_name, const char *function_name, unsigned int line_no,
                plLexicalScanner *scanner, int marker, plLexicalLocation *location);
#define EXPECT_MARKER(scanner, marker, location) \
    expectMarkerLog(__FILE__, __func__, __LINE__, scanner, marker, location)

#endif  // LL_USE == VASQ_LL_RAWONLY

int
expectMarker(plLexicalScanner *scanner, int marker, plLexicalLocation *location);

plAstNode *
createFamily(int marker, ...);

int
createConnection(int marker, plAstNode **first, plAstNode *second);

int
parseExtendedName(plLexicalScanner *scanner, plAstNode **node);

int
parseExtendedType(plLexicalScanner *scanner, plAstNode **node);

int
parseArrayDeclaration(plLexicalScanner *scanner, plAstNode **node);

int
parseStructDefinition(plLexicalScanner *scanner, plAstNode **node);

int
parseExpression(plLexicalScanner *scanner, plAstNode **node);

int
parseReceiver(plLexicalScanner *scanner, plAstNode **node);

int
parseIfBlock(plLexicalScanner *scanner, plAstNode **node);

int
parseWhileBlock(plLexicalScanner *scanner, plAstNode **node);

int
parseStatementList(plLexicalScanner *scanner, plAstNode **node);

int
parseFunction(plLexicalScanner *scanner, plAstNode **node);

int
parseInlineFunction(plLexicalScanner *scanner, plAstNode **node);
