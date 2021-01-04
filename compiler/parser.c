#include <string.h>

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
        COMPILER_ERROR("Unxpected %s after %s", plLexicalMarkerName(token.marker),
                       plLexicalMarkerName(marker));
        plTokenCleanup(&token, scanner->table);
        return PL_RET_BAD_DATA;
    }

    name_node = plAstNew(PL_MARKER_NAME);
    if (!name_node) {
        plTokenCleanup(&token, scanner->table);
        return PL_RET_OUT_OF_MEMORY;
    }
    memcpy(&name_node->token, &token, sizeof(token));

    *node = createFamily(marker, name_node);
    if (!*node) {
        plAstFree(name_node, scanner->table);
        return PL_RET_OUT_OF_MEMORY;
    }
    plAstSetLocation(*node, &location);

    ret = expectMarker(scanner, PL_MARKER_SEMICOLON, NULL);
    if (ret != PL_RET_OK) {
        plAstFree(*node, scanner->table);
    }
    return ret;
}

static int
parseConstantDeclaration(plLexicalScanner *scanner, plAstNode **node)
{
    (void)scanner;

    *node = NULL;
    VASQ_ERROR("This function has not yet been implemented.");
    return PL_RET_BAD_DATA;
}

static int
parseMain(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalLocation location;
    plAstNode *statement_list;

    *node = NULL;
    plGetLastLocation(scanner, &location);

    ret = expectMarker(scanner, PL_MARKER_LEFT_BRACE, NULL);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = parseStatementList(scanner, &statement_list);
    if (ret != PL_RET_OK) {
        return ret;
    }

    *node = createFamily(PL_MARKER_MAIN, statement_list);
    if (!*node) {
        plAstFree(statement_list, scanner->table);
        return PL_RET_OUT_OF_MEMORY;
    }
    plAstSetLocation(*node, &location);

    return PL_RET_OK;
}

static int
parseGlobalSpace(plLexicalScanner *scanner, plAstNode **tree)
{
    int ret;
    plLexicalToken token;

    *tree = NULL;

    while (!TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
        plAstNode *node = NULL;

        switch (scanner->last_marker) {
        case PL_MARKER_IMPORT:
        case PL_MARKER_EXPORT: ret = parseImportExport(scanner, &node); break;

        case PL_MARKER_SOURCE:
        case PL_MARKER_PIPE:
        case PL_MARKER_SINK: ret = parseFunction(scanner, &node); break;

        case PL_MARKER_STRUCT: ret = parseStructDefinition(scanner, &node); break;

        case PL_MARKER_MAIN: ret = parseMain(scanner, &node); break;

        default:
            ret = LOOKAHEAD_STORE(scanner, &token);
            if (ret != PL_RET_OK) {
                goto cleanup_token;
            }
            ret = parseConstantDeclaration(scanner, &node);
            break;
        }

        if (ret != PL_RET_OK) {
cleanup_token:
            plTokenCleanup(&token, scanner->table);
            goto error;
        }

        if (*tree) {
            ret = createConnection(PL_MARKER_SEMICOLON, tree, node);
            if (ret != PL_RET_OK) {
                plAstFree(node, scanner->table);
                goto error;
            }
        }
        else {
            *tree = node;
        }
    }

    if (scanner->last_marker == PL_MARKER_EOF) {
        if (!*tree) {
            COMPILER_ERROR("File is empty");
            return PL_RET_BAD_DATA;
        }
        return PL_RET_OK;
    }
    else {
        ret = plTranslateTerminalMarker(scanner->last_marker);
    }

error:

    plAstFree(*tree, scanner->table);
    *tree = NULL;

    return ret;
}

int
plFileParse(FILE *in, const char *file_name, plAstNode **tree, plNameTable **table)
{
    int ret;
    plLexicalScanner scanner;

    if (!in || !tree || !table) {
        VASQ_ERROR("in, tree, and table cannot be NULL");
        return PL_RET_USAGE;
    }

    *tree = NULL;

    *table = plNameTableNew();
    if (!*table) {
        return PL_RET_OUT_OF_MEMORY;
    }

    plScannerInit(&scanner, in, file_name, *table);
    VASQ_INFO("Parsing %s", scanner.file_name);

    ret = parseGlobalSpace(&scanner, tree);
    if (ret != PL_RET_OK) {
        plAstFree(*tree, *table);
        plScannerCleanup(&scanner);
        plNameTableFree(*table);
    }
    return ret;
}

#if LL_USE == VASQ_LL_RAWONLY

int
nextToken(plLexicalScanner *scanner, plLexicalToken *token)
{
    int marker;

    marker = plTokenRead(scanner, token);
    return TERMINAL_MARKER(marker) ? plTranslateTerminalMarker(marker) : PL_RET_OK;
}

#else  // LL_USE == VASQ_LL_RAWONLY

int
nextTokenLog(const char *file_name, const char *function_name, unsigned int line_no,
             plLexicalScanner *scanner, plLexicalToken *token)
{
    int marker;

    marker = plTokenReadLog(file_name, function_name, line_no, scanner, token);
    return TERMINAL_MARKER(marker) ? plTranslateTerminalMarker(marker) : PL_RET_OK;
}

#endif  // LL_USE == VASQ_LL_RAWONLY

int
expectMarker(plLexicalScanner *scanner, int marker, plLexicalLocation *location)
{
    int ret;
    plLexicalToken token;

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (token.marker != marker) {
        COMPILER_ERROR("Unexpected %s", plLexicalMarkerName(token.marker));
        plTokenCleanup(&token, scanner->table);
        return PL_RET_BAD_DATA;
    }

    if (location) {
        memcpy(location, &token.location, sizeof(token.location));
    }

    return PL_RET_OK;
}

plAstNode *
createFamily(int marker, ...)
{
    int split_size;
    va_list args;
    plAstNode *parent;
    plAstMaxSplitNode *splitter;

    parent = plAstNew(marker);
    if (!parent) {
        return NULL;
    }
    splitter = (plAstMaxSplitNode *)parent;

    split_size = plAstSplitSize(marker);
    va_start(args, marker);
    for (int k = 0; k < split_size; k++) {
        plAstNode *node;

        node = va_arg(args, plAstNode *);
#ifdef AST_HAS_PARENT
        if (node) {
            node->parent = parent;
        }
#endif
        splitter->nodes[k] = node;
    }
    va_end(args);

    return parent;
}

int
createConnection(int marker, plAstNode **first, plAstNode *second)
{
    plAstNode *parent;

    if (!first || !second) {
        VASQ_ERROR("The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    if (!*first) {
        VASQ_ERROR("*first cannot be NULL.");
        return PL_RET_USAGE;
    }

    parent = createFamily(marker, *first, second);
    if (!parent) {
        return PL_RET_OUT_OF_MEMORY;
    }
    *first = parent;

    return PL_RET_OK;
}
