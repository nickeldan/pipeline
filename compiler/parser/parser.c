#include "vasq/safe_snprintf.h"

#include "parserInternal.h"

static int
parseImportExportOpaque(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalToken lead_token, token;
    plAstNode *name_node;

    *node = NULL;

    ret = CONSUME_TOKEN(scanner, &lead_token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = CONSUME_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }
    if (token.header.marker != PL_MARKER_NAME) {
        PARSER_ERROR("Unexpected %s where NAME was expected", plLexicalMarkerName(token.header.marker));
        plTokenCleanup(&token, scanner->table);
        return PL_RET_BAD_DATA;
    }

    name_node = plAstNew(PL_MARKER_NAME, &token);
    *node = plAstCreateFamily(lead_token.header.marker, &lead_token, name_node);

    ret = EXPECT_MARKER(scanner, PL_MARKER_SEMICOLON, NULL);
    if (ret != PL_RET_OK) {
        plAstFree(*node, scanner->table);
        *node = NULL;
    }
    return ret;
}

static int
parseConstantDeclaration(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalToken arrow_token, name_token;
    plAstNode *name_node;

    ret = plParseExpression(scanner, node, true);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = EXPECT_MARKER(scanner, PL_MARKER_ARROW, &arrow_token);
    if (ret != PL_RET_OK) {
        goto error;
    }

    ret = EXPECT_MARKER(scanner, PL_MARKER_NAME, &name_token);
    if (ret != PL_RET_OK) {
        goto error;
    }
    name_node = plAstNew(PL_MARKER_NAME, &name_token);

    plAstCreateConnection(PL_MARKER_ARROW, &arrow_token, node, name_node);

    ret = EXPECT_MARKER(scanner, PL_MARKER_SEMICOLON, NULL);
    if (ret != PL_RET_OK) {
        goto error;
    }

    return PL_RET_OK;

error:

    plAstFree(*node, scanner->table);
    *node = NULL;

    return ret;
}

static int
parseMain(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalToken lead_token;
    plAstNode *statement_list;

    *node = NULL;
    ret = CONSUME_TOKEN(scanner, &lead_token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = EXPECT_MARKER(scanner, PL_MARKER_LEFT_BRACE, NULL);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = plParseStatementList(scanner, &statement_list);
    if (ret != PL_RET_OK) {
        return ret;
    }

    *node = plAstCreateFamily(PL_MARKER_MAIN, &lead_token, statement_list);

    return PL_RET_OK;
}

static int
parseGlobalSpace(plLexicalScanner *scanner, plAstNode **tree)
{
    int ret;

    *tree = NULL;

    while (!TERMINAL_MARKER(PEEK_TOKEN(scanner, 0))) {
        plAstNode *node;

        switch (PEEK_TOKEN(scanner, 0)) {
            plLexicalToken token;

        case PL_MARKER_IMPORT:
        case PL_MARKER_EXPORT:
        case PL_MARKER_OPAQUE: ret = parseImportExportOpaque(scanner, &node); break;

        case PL_MARKER_EXPORT_ALL:
            ret = CONSUME_TOKEN(scanner, &token);
            if (ret != PL_RET_OK) {
                goto error;
            }
            node = plAstNew(PL_MARKER_EXPORT_ALL, &token);
            break;

        case PL_MARKER_SOURCE:
        case PL_MARKER_PIPE:
        case PL_MARKER_SINK: ret = plParseFunction(scanner, &node, true); break;

        case PL_MARKER_STRUCT: ret = plParseStructDefinition(scanner, &node); break;

        case PL_MARKER_MAIN: ret = parseMain(scanner, &node); break;

        case PL_MARKER_SEMICOLON:
            ret = CONSUME_TOKEN(scanner, NULL);
            if (ret != PL_RET_OK) {
                goto error;
            }
            continue;

        default: ret = parseConstantDeclaration(scanner, &node); break;
        }

        if (ret != PL_RET_OK) {
            goto error;
        }
        if (*tree) {
            plAstCreateConnection(PL_MARKER_SEMICOLON, NULL, tree, node);
        }
        else {
            *tree = node;
        }
    }

    if (PEEK_TOKEN(scanner, 0) != PL_MARKER_EOF) {
        ret = plTranslateTerminalMarker(PEEK_TOKEN(scanner, 0));
        goto error;
    }

    if (!*tree) {
        PARSER_ERROR("File is empty.");
        return PL_RET_BAD_DATA;
    }

    return PL_RET_OK;

error:

    plAstFree(*tree, scanner->table);
    *tree = NULL;

    return ret;
}

int
plFileParse(FILE *in, const char *file_name, plAstNode **tree, plWordTable **table)
{
    int ret;
    plLexicalScanner scanner;

    if (UNLIKELY(!in || !tree || !table)) {
        VASQ_ERROR(debug_logger, "in, tree, and table cannot be NULL.");
        return PL_RET_USAGE;
    }

    ret = plScannerInit(&scanner, in, file_name);
    VASQ_INFO(debug_logger, "Parsing %s.", scanner.file_name);

    ret = parseGlobalSpace(&scanner, tree);
    if (ret == PL_RET_OK) {
        *table = scanner.table;
        scanner.table = NULL;
    }
    plScannerCleanup(&scanner);

    return ret;
}

int
plExpectMarker(
#if LL_USE != -1
    const char *file_name, const char *function_name, unsigned int line_no,
#endif
    plLexicalScanner *scanner, int marker, plLexicalToken *token)
{
    int ret;
    plLexicalToken temp_token;
    plLexicalToken *token_ptr = token ? token : &temp_token;

    if (UNLIKELY(!scanner)) {
        VASQ_ERROR(debug_logger, "scanner cannot be NULL.");
        return PL_RET_USAGE;
    }

#if LL_USE == -1
    ret = CONSUME_TOKEN(scanner, token_ptr);
#else
    ret = plTokenConsumeLog(file_name, function_name, line_no, scanner, token_ptr);
#endif
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (token_ptr->header.marker != marker) {
#if LL_USE == -1
        PARSER_ERROR(
#else
        vasqLogStatement(scanner->parser_logger, VASQ_LL_ERROR, file_name, function_name, line_no,
#endif
            "Expected %s instead of %s.", plLexicalMarkerName(marker),
            plLexicalMarkerName(token_ptr->header.marker));
        plTokenCleanup(token_ptr, scanner->table);
        return PL_RET_BAD_DATA;
    }

    return PL_RET_OK;
}
