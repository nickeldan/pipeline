#include <string.h>

#include "parser.h"

static int
parseExtendedName(plLexicalScanner *scanner, plAstNode **node);

static int
parseFunction(plLexicalScanner *scanner, plAstNode **node, int function_marker, bool allow_anonymous);

static int
parseStatementList(plLexicalScanner *scanner, plAstNode **node);

static int
translateTerminalMarker(int marker)
{
    switch (marker) {
    case PL_LMARKER_BAD_ARGS: return PL_RET_BAD_ARGS;
    case PL_LMARKER_READ_FAILURE: return PL_RET_IO;
    case PL_LMARKER_BAD_DATA: return PL_RET_BAD_DATA;
    case PL_LMARKER_OUT_OF_MEMORY: return PL_RET_OUT_OF_MEMORY;
    default: return PL_RET_BAD_DATA;  // PL_LMARKER_EOF
    }
}

static void
createFamily(plAstNode *parent, ...)
{
    int split_size;
    va_list args;
    plAstMaxSplitNode *splitter = (plAstMaxSplitNode *)parent;

    split_size = plAstSplitSize(parent->token.marker);
    va_start(args, parent);
    for (int k = 0; k < split_size; k++) {
        plAstNode *node;

        node = va_arg(args, plAstNode *);
        splitter->nodes[k] = node;
#ifdef AST_HAS_PARENT
        if (node) {
            node->parent = parent;
        }
#endif
    }
    va_end(args);
}

static int
parseImportExport(plLexicalScanner *scanner, plAstNode **node)
{
    int marker;
    plLexicalToken token;

    marker = scanner->last_marker;

    if (TERMINAL_LMARKER(TOKEN_READ(scanner, &token))) {
        return translateTerminalMarker(scanner->last_marker);
    }

    if (token.marker != PL_LMARKER_NAME) {
        COMPILER_ERROR("Unxpected %s after %s", plLexicalMarkerName(token.marker),
                     plLexicalMarkerName(marker));
        plTokenCleanup(&token, scanner->table);
        return PL_RET_BAD_DATA;
    }

    *node = plAstNew(token.marker);
    if (!*node) {
        plTokenCleanup(&token, scanner->table);
        return PL_RET_OUT_OF_MEMORY;
    }
    memcpy(&(*node)->token, &token, sizeof(token));

    if ( TERMINAL_LMARKER(TOKEN_READ(scanner, &token)) ) {
        plAstFree(*node, scanner->table);
        return translateTerminalMarker(scanner->last_marker);
    }

    if ( token.marker != PL_LMARKER_SEMICOLON ) {
        COMPILER_ERROR("Unexpected %s following NAME", plLexicalMarkerName(token.marker));
        plTokenCleanup(&token, scanner->table);
        plAstFree(*node, scanner->table);
        return PL_RET_BAD_DATA;
    }

    return PL_RET_OK;
}

static int
parseExtendedName(plLexicalScanner *scanner, plAstNode **node)
{
    int ret = PL_RET_BAD_DATA;
    plLexicalToken token;
    plAstNode *period_node = NULL;

    *node = NULL;
    while (true) {
        int previous_marker;
        plAstNode *name_node;

        previous_marker = scanner->last_marker;

        if (TERMINAL_LMARKER(TOKEN_READ(scanner, &token))) {
            goto error;
        }

        if (*node) {
            if (token.marker != PL_LMARKER_PERIOD) {
                LOOKAHEAD_STORE(scanner, &token);
                break;
            }

            period_node = plAstNew(PL_LMARKER_PERIOD);
            if (!period_node) {
                ret = PL_RET_OUT_OF_MEMORY;
                goto error;
            }

            if (TERMINAL_LMARKER(TOKEN_READ(scanner, &token))) {
                goto error;
            }
        }
        else {
            period_node = NULL;
        }

        if (token.marker != PL_LMARKER_NAME) {
            COMPILER_ERROR("Unexpected %s following %s", plLexicalMarkerName(token.marker),
                         plLexicalMarkerName(previous_marker));
            plTokenCleanup(&token, scanner->table);
            goto error;
        }

        name_node = plAstNew(PL_LMARKER_NAME);
        if (!name_node) {
            plTokenCleanup(&token, scanner->table);
            ret = PL_RET_OUT_OF_MEMORY;
            goto error;
        }
        memcpy(&name_node->token, &token, sizeof(token));

        if (period_node) {
            createFamily(period_node, *node, name_node);
            *node = period_node;
        }
        else {
            *node = name_node;
        }
    }

    return PL_RET_OK;

error:

    plAstFree(*node, scanner->table);
    plAstFree(period_node, scanner->table);

    if (TERMINAL_LMARKER(scanner->last_marker)) {
        return translateTerminalMarker(scanner->last_marker);
    }
    else {
        return ret;
    }
}

static int
parseExtendedType(plLexicalScanner *scanner, plAstNode **node)
{
    plLexicalToken token;

    if (TERMINAL_LMARKER(TOKEN_READ(scanner, &token))) {
        return translateTerminalMarker(scanner->last_marker);
    }

    if (token.marker == PL_LMARKER_TYPE) {
        *node = plAstNew(PL_LMARKER_TYPE);
        if (!*node) {
            plTokenCleanup(&token, scanner->table);
            return PL_RET_OUT_OF_MEMORY;
        }
        memcpy(&(*node)->token, &token, sizeof(token));
        return PL_RET_OK;
    }
    else {
        LOOKAHEAD_STORE(scanner, &token);
        return parseExtendedName(scanner, node);
    }
}

static int
parseFunction(plLexicalScanner *scanner, plAstNode **node, int function_marker, bool allow_anonymous)
{
    int ret = PL_RET_BAD_DATA;
    unsigned int starting_line_no;
    plLexicalToken token;
    plAstNode *function_name_node = NULL, *arg_list = NULL, *type_node = NULL, *statement_list;

    starting_line_no = scanner->line_no;

    if (TERMINAL_LMARKER(TOKEN_READ(scanner, &token))) {
        return translateTerminalMarker(scanner->last_marker);
    }

    if (token.marker == PL_LMARKER_NAME) {
        if (function_marker == PL_LMARKER_LOCAL) {
            COMPILER_ERROR("LOCAL cannot be named");
            plTokenCleanup(&token, scanner->table);
            return PL_RET_BAD_DATA;
        }

        function_name_node = plAstNew(PL_LMARKER_NAME);
        if (!function_name_node) {
            plTokenCleanup(&token, scanner->table);
            return PL_RET_OUT_OF_MEMORY;
        }
        memcpy(&function_name_node->token, &token, sizeof(token));

        if (TERMINAL_LMARKER(TOKEN_READ(scanner, &token))) {
            goto error;
        }
    }
    else if (!allow_anonymous) {
        COMPILER_ERROR("Anonymous %s not allowed in this context", plLexicalMarkerName(function_marker));
        plTokenCleanup(&token, scanner->table);
        return PL_RET_BAD_DATA;
    }

    if (token.marker != PL_LMARKER_LEFT_PARENS) {
        COMPILER_ERROR("Unexpected %s before argument list", plLexicalMarkerName(token.marker));
        plTokenCleanup(&token, scanner->table);
        goto error;
    }

    while (true) {
        int previous_marker = scanner->last_marker;
        plAstNode *name_node, *arg_node;

        if (TERMINAL_LMARKER(TOKEN_READ(scanner, &token))) {
            goto error;
        }

        if (token.marker == PL_LMARKER_RIGHT_PARENS) {
            if (previous_marker == PL_LMARKER_COMMA) {
                COMPILER_ERROR("Unexpected ')' following ',' in argument list");
                goto error;
            }
            break;
        }

        if (token.marker == PL_LMARKER_COMMA) {
            if (previous_marker == PL_LMARKER_LEFT_PARENS || previous_marker == PL_LMARKER_COMMA) {
                COMPILER_ERROR("Unexpected ',' following %s in argument list",
                             plLexicalMarkerName(previous_marker));
                goto error;
            }

            if (function_marker != PL_LMARKER_SOURCE) {
                COMPILER_ERROR("Multiple arguments are only allowed in a SOURCE");
                goto error;
            }

            continue;
        }

        if (token.marker != PL_LMARKER_NAME) {
            COMPILER_ERROR("Unexpected %s following %s in argunent list", plLexicalMarkerName(token.marker),
                         plLexicalMarkerName(previous_marker));
            plTokenCleanup(&token, scanner->table);
            goto error;
        }

        name_node = plAstNew(PL_LMARKER_NAME);
        if (!name_node) {
            plTokenCleanup(&token, scanner->table);
            ret = PL_RET_OUT_OF_MEMORY;
            goto error;
        }
        memcpy(&name_node->token, &token, sizeof(token));

        if (TERMINAL_LMARKER(TOKEN_READ(scanner, &token))) {
            plAstFree(name_node, scanner->table);
            goto error;
        }

        if (token.marker != PL_LMARKER_COLON) {
            COMPILER_ERROR("Unexpected %s following NAME in argument list", plLexicalMarkerName(token.marker));
            plTokenCleanup(&token, scanner->table);
            goto cleanup_name_node;
        }

        ret = parseExtendedType(scanner, &type_node);
        if ( ret != PL_RET_OK ) {
            goto cleanup_name_node;
        }
        ret = PL_RET_BAD_DATA;

        arg_node = plAstNew(PL_LMARKER_COLON);
        if (!arg_node) {
            ret = PL_RET_OUT_OF_MEMORY;
            goto cleanup_name_node;
        }
        createFamily(arg_node, name_node, type_node);
        type_node = NULL;

        if (arg_list) {
            plAstNode *comma_node;

            comma_node = plAstNew(PL_LMARKER_COMMA);
            if (!comma_node) {
                plAstFree(arg_node, scanner->table);
                ret = PL_RET_OUT_OF_MEMORY;
                goto error;
            }
            createFamily(comma_node, arg_list, arg_node);
            arg_list = comma_node;
        }
        else {
            arg_list = arg_node;
        }

        continue;

    cleanup_name_node:

        plAstFree(name_node, scanner->table);
        goto error;
    }

    if (function_marker != PL_LMARKER_SINK) {
        if (TERMINAL_LMARKER(TOKEN_READ(scanner, &token))) {
            goto error;
        }

        if (token.marker != PL_LMARKER_ARROW) {
            COMPILER_ERROR("Unexpected %s following argument list", plLexicalMarkerName(token.marker));
            plTokenCleanup(&token, scanner->table);
            goto error;
        }

        ret = parseExtendedType(scanner, &type_node);
        if (ret != PL_RET_OK) {
            goto error;
        }
        ret = PL_RET_BAD_DATA;
    }

    if (TERMINAL_LMARKER(TOKEN_READ(scanner, &token))) {
        goto error;
    }

    if (token.marker != PL_LMARKER_LEFT_BRACE) {
        COMPILER_ERROR("Unexpected %s following %s header", plLexicalMarkerName(token.marker),
                     plLexicalMarkerName(function_marker));
        plTokenCleanup(&token, scanner->table);
        goto error;
    }

    ret = parseStatementList(scanner, &statement_list);
    if (ret != PL_RET_OK) {
        goto error;
    }

    if ( TERMINAL_LMARKER(TOKEN_READ(scanner, &token)) ) {
        plAstFree(statement_list, scanner->table);
        goto error;
    }

    if ( token.marker != PL_LMARKER_RIGHT_BRACE ) {
        COMPILER_ERROR("Expected RIGHT_BRACE at end of %s definition which began on line %u", plLexicalMarkerName(function_marker), starting_line_no);
        plTokenCleanup(&token, scanner->table);
        plAstFree(statement_list, scanner->table);
        goto error;
    }

    *node = plAstNew(function_marker);
    if (!*node) {
        plAstFree(statement_list, scanner->table);
        goto error;
    }

    switch (function_marker) {
    case PL_LMARKER_SOURCE:
    case PL_LMARKER_PIPE: createFamily(*node, function_name_node, arg_list, type_node, statement_list); break;

    case PL_LMARKER_SINK: createFamily(*node, function_name_node, arg_list, statement_list); break;

    default:  // PL_LMARKER_LOCAL
        createFamily(*node, arg_list, statement_list);
        break;
    }

    return PL_RET_OK;

error:

    plAstFree(function_name_node, scanner->table);
    plAstFree(arg_list, scanner->table);
    plAstFree(type_node, scanner->table);

    if (TERMINAL_LMARKER(scanner->last_marker)) {
        return translateTerminalMarker(scanner->last_marker);
    }
    else {
        return ret;
    }
}

static int
parseStatementList(plLexicalScanner *scanner, plAstNode **node)
{
    (void)scanner;
    (void)node;
    return PL_RET_BAD_DATA; // placeholder;
}

static int
parseMain(plLexicalScanner *scanner, plAstNode **node)
{
    (void)scanner;
    (void)node;
    return PL_RET_BAD_DATA; // placeholder;
}

static int
parseGlobalSpace(plLexicalScanner *scanner, plAstNode **tree)
{
    int ret;
    plLexicalToken token;

    while (!TERMINAL_LMARKER(TOKEN_READ(scanner, &token))) {
        plAstNode *node;

        switch (scanner->last_marker) {
        case PL_LMARKER_IMPORT:
        case PL_LMARKER_EXPORT: ret = parseImportExport(scanner, &node); break;

        case PL_LMARKER_SOURCE:
        case PL_LMARKER_PIPE:
        case PL_LMARKER_SINK: ret = parseFunction(scanner, &node, scanner->last_marker, false); break;

        case PL_LMARKER_MAIN: ret = parseMain(scanner, &node); break;

        default:
            COMPILER_ERROR("Invalid token in global namespace: %s", plLexicalMarkerName(token.marker));
            plTokenCleanup(&token, scanner->table);
            return PL_RET_BAD_DATA;
        }

        if (ret != PL_RET_OK) {
            return ret;
        }

        if (*tree) {
            plAstNode *parent;

            parent = plAstNew(PL_LMARKER_SEMICOLON);
            if (!parent) {
                plAstFree(*tree, scanner->table);
                plAstFree(node, scanner->table);
                return PL_RET_OUT_OF_MEMORY;
            }

            createFamily(parent, *tree, node);
            *tree = parent;
        }
        else {
            *tree = node;
        }
    }

    if ( scanner->last_marker == PL_LMARKER_EOF ) {
        if ( !*tree ) {
            COMPILER_ERROR("File is empty");
            return PL_RET_BAD_DATA;
        }
        return PL_RET_OK;
    }

    return translateTerminalMarker(scanner->last_marker);
}

int
plFileParse(FILE *in, const char *file_name, plAstNode **tree, plNameTable **table)
{
    int ret;
    plLexicalScanner scanner;

    if (!in || !tree || !table) {
        VASQ_ERROR("in, tree, and table cannot be NULL");
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
        plNameTableFree(*table);
    }
    return ret;
}
