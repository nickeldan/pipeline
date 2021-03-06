#include <string.h>

#include "vasq/safe_snprintf.h"

#include "parserInternal.h"

static int
parseImportExport(plLexicalScanner *scanner, plAstNode **node)
{
    int ret, marker;
    plLexicalLocation location;
    plLexicalToken token;
    plAstNode *name_node;

    *node = NULL;
    marker = scanner->last_marker;
    plGetLastLocation(scanner, &location);

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (token.marker != PL_MARKER_NAME) {
        PARSER_ERROR("Expected %s intead of %s.", plLexicalMarkerName(PL_MARKER_NAME),
                     plLexicalMarkerName(token.marker));
        plTokenCleanup(&token, scanner->table);
        return PL_RET_BAD_DATA;
    }

    name_node = plAstNew(PL_MARKER_NAME);
    if (!name_node) {
        plTokenCleanup(&token, scanner->table);
        return PL_RET_OUT_OF_MEMORY;
    }
    memcpy(&name_node->token, &token, sizeof(token));

    *node = plAstCreateFamily(marker, name_node);
    if (!*node) {
        plAstFree(name_node, scanner->table);
        return PL_RET_OUT_OF_MEMORY;
    }
    memcpy(&(*node)->token.location, &location, sizeof(location));

    ret = EXPECT_MARKER(scanner, PL_MARKER_SEMICOLON, NULL);
    if (ret != PL_RET_OK) {
        plAstFree(*node, scanner->table);
    }
    return ret;
}

static int
parseConstantDeclaration(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalLocation location;
    plLexicalToken token;
    plAstNode *name_node;

    ret = parseExpression(scanner, node, true);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = EXPECT_MARKER(scanner, PL_MARKER_ARROW, &location);
    if (ret != PL_RET_OK) {
        goto error;
    }

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        goto error;
    }
    if (token.marker != PL_MARKER_NAME) {
        PARSER_ERROR("Unexpected %s in place of NAME.", plLexicalMarkerName(token.marker));
        plTokenCleanup(&token, scanner->table);
        ret = PL_RET_BAD_DATA;
        goto error;
    }
    name_node = plAstNew(PL_MARKER_NAME);
    if (!name_node) {
        plTokenCleanup(&token, scanner->table);
        ret = PL_RET_OUT_OF_MEMORY;
        goto error;
    }
    memcpy(&name_node->token, &token, sizeof(token));

    ret = plAstCreateConnection(PL_MARKER_ARROW, node, name_node);
    if (ret != PL_RET_OK) {
        plAstFree(name_node, scanner->table);
        goto error;
    }
    memcpy(&(*node)->token.location, &location, sizeof(location));

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
parseTypeDecl(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalLocation location;
    plLexicalToken token;
    plAstNode *name_node, *type_node;

    *node = NULL;
    plGetLastLocation(scanner, &location);

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (token.marker != PL_MARKER_NAME) {
        PARSER_ERROR("Unexpected %s in place of NAME.", plLexicalMarkerName(token.marker));
        plTokenCleanup(&token, scanner->table);
        return PL_RET_BAD_DATA;
    }

    name_node = plAstNew(PL_MARKER_NAME);
    if (!name_node) {
        plTokenCleanup(&token, scanner->table);
        return PL_RET_OUT_OF_MEMORY;
    }
    memcpy(&name_node->token, &token, sizeof(token));

    ret = parseExtendedType(scanner, &type_node);
    if (ret != PL_RET_OK) {
        goto error;
    }

    ret = EXPECT_MARKER(scanner, PL_MARKER_SEMICOLON, NULL);
    if (ret != PL_RET_OK) {
        goto error;
    }

    *node = plAstCreateFamily(PL_MARKER_TYPE_DECL, name_node, type_node);
    if (!*node) {
        ret = PL_RET_OUT_OF_MEMORY;
        goto error;
    }
    memcpy(&(*node)->token.location, &location, sizeof(location));

    return PL_RET_OK;

error:

    plAstFree(name_node, scanner->table);
    plAstFree(type_node, scanner->table);

    return ret;
}

static int
parseMain(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalLocation location;
    plAstNode *statement_list;

    *node = NULL;
    plGetLastLocation(scanner, &location);

    ret = EXPECT_MARKER(scanner, PL_MARKER_LEFT_BRACE, NULL);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = parseStatementList(scanner, &statement_list);
    if (ret != PL_RET_OK) {
        return ret;
    }

    *node = plAstCreateFamily(PL_MARKER_MAIN, statement_list);
    if (!*node) {
        plAstFree(statement_list, scanner->table);
        return PL_RET_OUT_OF_MEMORY;
    }
    memcpy(&(*node)->token.location, &location, sizeof(location));

    return PL_RET_OK;
}

static int
parseGlobalSpace(plLexicalScanner *scanner, plAstNode **tree)
{
    int ret;
    plLexicalToken token;

    *tree = NULL;

    while (!TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
        plAstNode *node;

        switch (scanner->last_marker) {
        case PL_MARKER_IMPORT:
        case PL_MARKER_EXPORT: ret = parseImportExport(scanner, &node); break;

        case PL_MARKER_EXPORT_ALL:
            node = plAstNew(PL_MARKER_EXPORT_ALL);
            if (node) {
                memcpy(&node->token.location, &token.location, sizeof(token.location));
                ret = PL_RET_OK;
            }
            else {
                ret = PL_RET_OUT_OF_MEMORY;
            }
            break;

        case PL_MARKER_SOURCE:
        case PL_MARKER_PIPE:
        case PL_MARKER_SINK: ret = parseFunction(scanner, &node, true); break;

        case PL_MARKER_STRUCT: ret = parseStructDefinition(scanner, &node); break;

        case PL_MARKER_TYPE_DECL: ret = parseTypeDecl(scanner, &node); break;

        case PL_MARKER_MAIN: ret = parseMain(scanner, &node); break;

        case PL_MARKER_SEMICOLON: continue;

        default:
            ret = LOOKAHEAD_STORE(scanner, &token);
            if (ret != PL_RET_OK) {
                goto error;
            }
            ret = parseConstantDeclaration(scanner, &node);
            break;
        }

        if (ret != PL_RET_OK) {
            goto error;
        }

        if (*tree) {
            ret = plAstCreateConnection(PL_MARKER_SEMICOLON, tree, node);
            if (ret != PL_RET_OK) {
                plAstFree(node, scanner->table);
                goto error;
            }
        }
        else {
            *tree = node;
        }
    }

    if (scanner->last_marker != PL_MARKER_EOF) {
        ret = plTranslateTerminalMarker(scanner->last_marker);
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

    if (!in || !tree || !table) {
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

#if LL_USE == -1

int
nextTokenNoLog(plLexicalScanner *scanner, plLexicalToken *token)
{
    int marker;

    marker = plTokenRead(scanner, token);
    return TERMINAL_MARKER(marker) ? plTranslateTerminalMarker(marker) : PL_RET_OK;
}

int
expectMarkerNoLog(plLexicalScanner *scanner, int marker, plLexicalLocation *location)
{
    int ret;
    plLexicalToken token;

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (token.marker != marker) {
        PARSER_ERROR("Expected %s instead of %s.", plLexicalMarkerName(marker),
                     plLexicalMarkerName(token.marker));
        plTokenCleanup(&token, scanner->table);
        return PL_RET_BAD_DATA;
    }

    if (location) {
        memcpy(location, &token.location, sizeof(token.location));
    }

    return PL_RET_OK;
}

#else  // LL_USE == -1

int
nextTokenLog(const char *file_name, const char *function_name, unsigned int line_no,
             plLexicalScanner *scanner, plLexicalToken *token)
{
    int marker;

    marker = plTokenReadLog(file_name, function_name, line_no, scanner, token);
    return TERMINAL_MARKER(marker) ? plTranslateTerminalMarker(marker) : PL_RET_OK;
}

int
expectMarkerLog(const char *file_name, const char *function_name, unsigned int line_no,
                plLexicalScanner *scanner, int marker, plLexicalLocation *location)
{
    int ret;
    plLexicalToken token;

    ret = nextTokenLog(file_name, function_name, line_no, scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (token.marker != marker) {
        vasqLogStatement(scanner->parser_logger, VASQ_LL_ERROR, file_name, function_name, line_no,
                         "Unexpected %s.", plLexicalMarkerName(token.marker));
        plTokenCleanup(&token, scanner->table);
        return PL_RET_BAD_DATA;
    }

    if (location) {
        memcpy(location, &token.location, sizeof(token.location));
    }

    return PL_RET_OK;
}

#endif  // LL_USE == -1
