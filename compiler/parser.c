#include <string.h>

#include "parser.h"

static int
parseExpression(plLexicalScanner *scanner, plAstNode **node);

static int
parseIfBlock(plLexicalScanner *scanner, plAstNode **node);

static int
parseWhileBlock(plLexicalScanner *scanner, plAstNode **node);

static int
parseStatement(plLexicalScanner *scanner, plAstNode **node);

static int
parseStatementList(plLexicalScanner *scanner, plAstNode **node);

static int
parseFunction(plLexicalScanner *scanner, plAstNode **node, bool allow_anonymous);

static int
translateTerminalMarker(int marker)
{
    switch (marker) {
    case PL_MARKER_BAD_ARGS: return PL_RET_USAGE;
    case PL_MARKER_READ_FAILURE: return PL_RET_IO;
    case PL_MARKER_BAD_DATA: return PL_RET_BAD_DATA;
    case PL_MARKER_OUT_OF_MEMORY: return PL_RET_OUT_OF_MEMORY;
    default: return PL_RET_BAD_DATA;  // PL_MARKER_EOF
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
expectMarker(plLexicalScanner *scanner, int marker)
{
    plLexicalToken token;

    if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
        return translateTerminalMarker(scanner->last_marker);
    }

    if (token.marker != marker) {
        COMPILER_ERROR("Unexpected %s", plLexicalMarkerName(token.marker));
        plTokenCleanup(&token, scanner->table);
        return PL_RET_BAD_DATA;
    }

    return PL_RET_OK;
}

static int
parseImportExport(plLexicalScanner *scanner, plAstNode **node)
{
    int ret, marker;
    plLexicalToken token;

    marker = scanner->last_marker;

    if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
        return translateTerminalMarker(scanner->last_marker);
    }

    if (token.marker != PL_MARKER_NAME) {
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

    ret = expectMarker(scanner, PL_MARKER_SEMICOLON);
    if (ret != PL_RET_OK) {
        plAstFree(*node, scanner->table;
    }
    return ret;
}

static int
parseExtendedName(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalToken token;
    plAstNode *period_node = NULL;

    *node = NULL;
    while (true) {
        int previous_marker;
        plAstNode *name_node;

        previous_marker = scanner->last_marker;

        if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
            goto error;
        }

        if (*node) {
            if (token.marker != PL_MARKER_PERIOD) {
                ret = LOOKAHEAD_STORE(scanner, &token);
                if (ret != PL_RET_OK) {
                    plTokenCleanup(&token, scanner->table);
                    goto error;
                }
                break;
            }

            period_node = plAstNew(PL_MARKER_PERIOD);
            if (!period_node) {
                ret = PL_RET_OUT_OF_MEMORY;
                goto error;
            }

            if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
                goto error;
            }
        }
        else {
            period_node = NULL;
        }

        if (token.marker != PL_MARKER_NAME) {
            COMPILER_ERROR("Unexpected %s following %s", plLexicalMarkerName(token.marker),
                           plLexicalMarkerName(previous_marker));
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

    if (TERMINAL_MARKER(scanner->last_marker)) {
        return translateTerminalMarker(scanner->last_marker);
    }
    else {
        return ret;
    }
}

static int
parseExtendedType(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalToken token;

    if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
        return translateTerminalMarker(scanner->last_marker);
    }

    if (token.marker == PL_MARKER_TYPE) {
        *node = plAstNew(PL_MARKER_TYPE);
        if (!*node) {
            plTokenCleanup(&token, scanner->table);
            return PL_RET_OUT_OF_MEMORY;
        }
        memcpy(&(*node)->token, &token, sizeof(token));
        return PL_RET_OK;
    }
    else {
        ret = LOOKAHEAD_STORE(scanner, &token);
        if (ret != PL_RET_OK) {
            plTokenCleanup(&token, scanner->table);
            return ret;
        }
        return parseExtendedName(scanner, node);
    }
}

static int
parseExpression(plLexicalScanner *scanner, plAstNode **node)
{
    (void)scanner;
    (void)node;
    VASQ_ERROR("This function has not yet been implemented");
    return PL_RET_BAD_DATA;  // placeholder
}

static int
parseCondition(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    bool condition_has_parens;
    plLexicalToken token;

    *node = NULL;

    if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
        return translateTerminalMarker(token.marker);
    }

    if (token.marker == PL_MARKER_LEFT_PARENS) {
        condition_has_parens = true;
    }
    else {
        condition_has_parens = false;
        ret = LOOKAHEAD_STORE(scanner, &token);
        if (ret != PL_RET_OK) {
            plTokenCleanup(&token, scanner->table);
            return ret;
        }
    }

    ret = parseExpression(scanner, node);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (condition_has_parens) {
        ret = expectMarker(scanner, PL_MARKER_RIGHT_PARENS);
        if (ret != PL_RET_OK) {
            plAstFree(*node, scanner->table);
            return ret;
        }
    }

    ret = expectMarker(scanner, PL_MARKER_LEFT_BRACE);
    if (ret != PL_RET_OK) {
        plAstFree(*node, scanner->table);
    }

    return ret;
}

static int
parseIfBlock(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalToken token;
    plAstNode *condition_node, *statement_list = NULL, *eif_node = NULL, *last_eif_node = NULL,
                               *else_node = NULL;

    ret = parseCondition(scanner, &condition_node);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = parseStatementList(scanner, &statement_list);
    if (ret != PL_RET_OK) {
        goto error;
    }

    while (TOKEN_READ(scanner, &token) == PL_MARKER_EIF) {
        plAstNode *eif2_node, *eif_condition_node, *eif_statement_list = NULL;

        ret = parseCondition(scanner, &eif_condition_node);
        if (ret != PL_RET_OK) {
            goto error;
        }

        ret = parseStatementList(scanner, &eif_statement_list);
        if (ret != PL_RET_OK) {
            goto eif_loop_error;
        }

        eif2_node = plAstNew(PL_MARKER_EIF);
        if (!eif2_node) {
            ret = PL_RET_OUT_OF_MEMORY;
            goto eif_loop_error;
        }
        createFamily(eif2_node, eif_condition_node, eif_statement_list, NULL);

        if (last_eif_node) {
            ((plAstThreeSplitNode *)last_eif_node)->nodes[2] = eif2_node;
        }
        else {
            eif_node = eif2_node;
        }
        last_eif_node = eif2_node;

        continue;

eif_loop_error:

        plAstFree(eif_condition_node, scanner->table);
        plAstFree(eif_statement_list, scanner->table);
        goto error;
    }
    if (TERMINAL_MARKER(scanner->last_marker)) {
        goto error;
    }

    if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
        goto error;
    }

    if (token.marker == PL_MARKER_ELSE) {
        plAstNode *else_statement_list;

        ret = expectMarker(scanner, PL_MARKER_LEFT_BRACE);
        if (ret != PL_RET_OK) {
            goto error;
        }

        ret = parseStatementList(scanner, &else_statement_list);
        if (ret != PL_RET_OK) {
            goto error;
        }

        else_node = plAstNew(PL_MARKER_ELSE);
        if (!else_node) {
            plAstFree(else_statement_list, scanner->table);
            ret = PL_RET_OUT_OF_MEMORY;
            goto error;
        }
        createFamily(else_node, else_statement_list);
    }
    else {
        ret = LOOKAHEAD_STORE(scanner, &token);
        if (ret != PL_RET_OK) {
            plTokenCleanup(&token, scanner->table);
            goto error;
        }
    }

    *node = plAstNew(PL_MARKER_IF);
    if (!*node) {
        ret = PL_RET_OUT_OF_MEMORY;
        goto error;
    }
    createFamily(*node, condition_node, statement_list, eif_node, else_node);

    return PL_RET_OK;

error:

    plAstFree(condition_node, scanner->table);
    plAstFree(statement_list, scanner->table);
    plAstFree(eif_node, scanner->table);
    plAstFree(else_node, scanner->table);

    if (TERMINAL_MARKER(scanner->last_marker)) {
        return translateTerminalMarker(scanner->last_marker);
    }
    else {
        return ret;
    }
}

static int
parseWhileBlock(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plAstNode *condition_node, *statement_list;

    ret = parseCondition(scanner, &condition_node);
    if (ret != PL_RET_OK) {
        return ret;
    }

    ret = parseStatementList(scanner, &statement_list);
    if (ret != PL_RET_OK) {
        plAstFree(condition_node, scanner->table);
        return ret;
    }

    *node = plAstNew(PL_MARKER_WHILE);
    if (!*node) {
        plAstFree(condition_node, scanner->table);
        plAstFree(statement_list, scanner->table);
        return PL_RET_OUT_OF_MEMORY;
    }
    createFamily(*node, condition_node, statement_list);

    return PL_RET_OK;
}

static int
parseStatement(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalToken token;

    if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
        return translateTerminalMarker(scanner->last_marker);
    }

    switch (token.marker) {
        plAstNode *expression_node;

    case PL_MARKER_DROP:
    case PL_MARKER_END:
    case PL_MARKER_BREAK:
    case PL_MARKER_CONT: *node = plAstNew(token.marker); return (*node) ? PL_RET_OK : PL_RET_OUT_OF_MEMORY;

    case PL_MARKER_PROD:
    case PL_MARKER_VERIFY:
    case PL_MARKER_ABORT:
        ret = parseExpression(scanner, &expression_node);
        if (ret != PL_RET_OK) {
            return ret;
        }

        *node = plAstNew(token.marker);
        if (!*node) {
            plAstFree(expression_node, scanner->table);
            return PL_RET_OUT_OF_MEMORY;
        }
        createFamily(*node, expression_node);

        return PL_RET_OK;

    case PL_MARKER_IF: return parseIfBlock(scanner, node);

    case PL_MARKER_WHILE: return parseWhileBlock(scanner, node);

    default: break;
    }

    if (token.marker == PL_MARKER_NAME) {
        plLexicalToken next_token;

        if (TERMINAL_MARKER(TOKEN_READ(scanner, &next_token))) {
            plTokenCleanup(&token, scanner->table);
            return translateTerminalMarker(scanner->last_marker);
        }

        if (next_token.marker == PL_MARKER_IS || next_token.marker == PL_MARKER_AS) {
            plAstNode *name_node, *type_node;

            ret = parseExtendedType(scanner, &type_node);
            if (ret != PL_RET_OK) {
                plTokenCleanup(&token, scanner->table);
                return ret;
            }

            ret = expectMarker(scanner, PL_MARKER_SEMICOLON);
            if (ret != PL_RET_OK) {
                plTokenCleanup(&token, scanner->table);
                plAstFree(type_node, scanner->table);
                return ret;
            }

            name_node = plAstNew(PL_MARKER_NAME);
            if (!name_node) {
                plTokenCleanup(&token, scanner->table);
                plAstFree(type_node, scanner->table);
                return PL_RET_OUT_OF_MEMORY;
            }
            memcpy(&name_node->token, &token, sizeof(token));

            *node = plAstNew(next_token.marker);
            if (*node) {
                plAstFree(name_node, scanner->table);
                plAstFree(type_node, scanner->table);
                return PL_RET_OUT_OF_MEMORY;
            }
            createFamily(*node, name_node, type_node);
            return PL_RET_OK;
        }
        else {
            ret = LOOKAHEAD_STORE(scanner, &next_token);
            if (ret != PL_RET_OK) {
                plTokenCleanup(&token, scanner->table);
                plTokenCleanup(&next_token, scanner->table);
                return ret;
            }
        }
    }

    VASQ_ERROR("This section of code has not yet been implemented");
    return PL_RET_BAD_DATA;  // placeholder

error:

    if (TERMINAL_MARKER(scanner->last_marker)) {
        return translateTerminalMarker(scanner->last_marker);
    }
    else {
        return ret;
    }
}

static int
parseStatementList(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalToken token;

    *node = NULL;
    while (true) {
        plAstNode *statement_node;

        if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
            goto error;
        }

        if (token.marker == PL_MARKER_RIGHT_BRACE) {
            break;
        }

        ret = LOOKAHEAD_STORE(scanner, &token);
        if (ret != PL_RET_OK) {
            plTokenCleanup(&token, scanner->table);
            goto error;
        }

        ret = parseStatement(scanner, &statement_node);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (*node) {
            plAstNode *semicolon_node;

            semicolon_node = plAstNew(PL_MARKER_SEMICOLON);
            if (!semicolon_node) {
                plAstFree(statement_node, scanner->table);
                ret = PL_RET_OUT_OF_MEMORY;
                goto error;
            }
            createFamily(semicolon_node, *node, statement_node);
            *node = semicolon_node;
        }
        else {
            *node = statement_node;
        }
    }

    return PL_RET_OK;

error:

    plAstFree(*node, scanner->table);

    if (TERMINAL_MARKER(scanner->last_marker)) {
        return translateTerminalMarker(scanner->last_marker);
    }
    else {
        return ret;
    }
}

static int
parseFunction(plLexicalScanner *scanner, plAstNode **node, bool allow_anonymous)
{
    int ret = PL_RET_BAD_DATA, function_marker = scanner->last_marker;
    plLexicalToken token;
    plAstNode *function_name_node = NULL, *arg_list = NULL, *type_node = NULL, *statement_list;

    *node = NULL;

    if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
        return translateTerminalMarker(scanner->last_marker);
    }

    if (token.marker == PL_MARKER_NAME) {
        if (function_marker == PL_MARKER_LOCAL) {
            COMPILER_ERROR("LOCAL cannot be named");
            plTokenCleanup(&token, scanner->table);
            return PL_RET_BAD_DATA;
        }

        function_name_node = plAstNew(PL_MARKER_NAME);
        if (!function_name_node) {
            plTokenCleanup(&token, scanner->table);
            return PL_RET_OUT_OF_MEMORY;
        }
        memcpy(&function_name_node->token, &token, sizeof(token));

        if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
            goto error;
        }
    }
    else if (!allow_anonymous) {
        COMPILER_ERROR("Anonymous %s not allowed in this context", plLexicalMarkerName(function_marker));
        plTokenCleanup(&token, scanner->table);
        return PL_RET_BAD_DATA;
    }

    if (token.marker != PL_MARKER_LEFT_PARENS) {
        COMPILER_ERROR("Unexpected %s before argument list", plLexicalMarkerName(token.marker));
        plTokenCleanup(&token, scanner->table);
        goto error;
    }

    while (true) {
        int previous_marker = scanner->last_marker;
        plAstNode *name_node, *arg_node;

        if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
            goto error;
        }

        if (token.marker == PL_MARKER_RIGHT_PARENS) {
            if (previous_marker == PL_MARKER_COMMA) {
                COMPILER_ERROR("Unexpected ')' following ',' in argument list");
                goto error;
            }
            break;
        }

        if (token.marker == PL_MARKER_COMMA) {
            if (previous_marker == PL_MARKER_LEFT_PARENS || previous_marker == PL_MARKER_COMMA) {
                COMPILER_ERROR("Unexpected ',' following %s in argument list",
                               plLexicalMarkerName(previous_marker));
                goto error;
            }

            if (function_marker != PL_MARKER_SOURCE) {
                COMPILER_ERROR("Multiple arguments are only allowed in a SOURCE");
                goto error;
            }

            continue;
        }

        if (token.marker != PL_MARKER_NAME) {
            COMPILER_ERROR("Unexpected %s following %s in argunent list", plLexicalMarkerName(token.marker),
                           plLexicalMarkerName(previous_marker));
            plTokenCleanup(&token, scanner->table);
            goto error;
        }

        name_node = plAstNew(PL_MARKER_NAME);
        if (!name_node) {
            plTokenCleanup(&token, scanner->table);
            ret = PL_RET_OUT_OF_MEMORY;
            goto error;
        }
        memcpy(&name_node->token, &token, sizeof(token));

        if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
            plAstFree(name_node, scanner->table);
            goto error;
        }

        if (token.marker != PL_MARKER_COLON) {
            COMPILER_ERROR("Unexpected %s following NAME in argument list",
                           plLexicalMarkerName(token.marker));
            plTokenCleanup(&token, scanner->table);
            goto cleanup_name_node;
        }

        ret = parseExtendedType(scanner, &type_node);
        if (ret != PL_RET_OK) {
            goto cleanup_name_node;
        }
        ret = PL_RET_BAD_DATA;

        arg_node = plAstNew(PL_MARKER_COLON);
        if (!arg_node) {
            ret = PL_RET_OUT_OF_MEMORY;
            goto cleanup_name_node;
        }
        createFamily(arg_node, name_node, type_node);
        type_node = NULL;

        if (arg_list) {
            plAstNode *comma_node;

            comma_node = plAstNew(PL_MARKER_COMMA);
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

    if (function_marker != PL_MARKER_SINK) {
        if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
            goto error;
        }

        if (token.marker != PL_MARKER_ARROW) {
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

    if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
        goto error;
    }

    if (token.marker != PL_MARKER_LEFT_BRACE) {
        COMPILER_ERROR("Unexpected %s following %s header", plLexicalMarkerName(token.marker),
                       plLexicalMarkerName(function_marker));
        plTokenCleanup(&token, scanner->table);
        goto error;
    }

    ret = parseStatementList(scanner, &statement_list);
    if (ret != PL_RET_OK) {
        goto error;
    }

    *node = plAstNew(function_marker);
    if (!*node) {
        plAstFree(statement_list, scanner->table);
        goto error;
    }

    switch (function_marker) {
    case PL_MARKER_SOURCE:
    case PL_MARKER_PIPE: createFamily(*node, function_name_node, arg_list, type_node, statement_list); break;

    case PL_MARKER_SINK: createFamily(*node, function_name_node, arg_list, statement_list); break;

    default:  // PL_MARKER_LOCAL
        createFamily(*node, arg_list, statement_list);
        break;
    }

    return PL_RET_OK;

error:

    plAstFree(function_name_node, scanner->table);
    plAstFree(arg_list, scanner->table);
    plAstFree(type_node, scanner->table);

    if (TERMINAL_MARKER(scanner->last_marker)) {
        return translateTerminalMarker(scanner->last_marker);
    }
    else {
        return ret;
    }
}

static int
parseMain(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalToken token;
    plAstNode *statement_list;

    if (TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
        return translateTerminalMarker(scanner->last_marker);
    }

    if (token.marker != PL_MARKER_LEFT_BRACE) {
        COMPILER_ERROR("Unexpected %s following MAIN", plLexicalMarkerName(token.marker));
        plTokenCleanup(&token, scanner->table);
        return PL_RET_BAD_DATA;
    }

    ret = parseStatementList(scanner, &statement_list);
    if (ret != PL_RET_OK) {
        return ret;
    }

    *node = plAstNew(PL_MARKER_MAIN);
    if (!*node) {
        plAstFree(statement_list, scanner->table);
        return PL_RET_OUT_OF_MEMORY;
    }
    createFamily(*node, statement_list);

    return PL_RET_OK;
}

static int
parseGlobalSpace(plLexicalScanner *scanner, plAstNode **tree)
{
    int ret;
    plLexicalToken token;

    while (!TERMINAL_MARKER(TOKEN_READ(scanner, &token))) {
        plAstNode *node;

        switch (scanner->last_marker) {
        case PL_MARKER_IMPORT:
        case PL_MARKER_EXPORT: ret = parseImportExport(scanner, &node); break;

        case PL_MARKER_SOURCE:
        case PL_MARKER_PIPE:
        case PL_MARKER_SINK: ret = parseFunction(scanner, &node, false); break;

        case PL_MARKER_MAIN: ret = parseMain(scanner, &node); break;

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

            parent = plAstNew(PL_MARKER_SEMICOLON);
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

    if (scanner->last_marker == PL_MARKER_EOF) {
        if (!*tree) {
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
