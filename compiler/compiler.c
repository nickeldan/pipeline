#include "vasq/logger.h"

#include "compiler.h"

#define PARSER_ERROR(format, ...) VASQ_RAWLOG("%s:%u: " format "\n", scanner->file_name, scanner->line_no, ##__VA_ARGS__)

static int
translateTerminalMarker(plLexicalMarker_t marker)
{
    switch ( marker ) {
    case PL_LMARKER_BAD_ARGS: return PL_RET_BAD_ARGS;
    case PL_LMARKER_READ_FAILURE: return PL_RET_IO;
    case PL_LMARKER_BAD_DATA: return PL_RET_BAD_DATA;
    case PL_LMARKER_OUT_OF_MEMORY: return PL_RET_OUT_OF_MEMORY;
    default: return PL_RET_OK; // PL_LMARKER_EOF
    }
}

static void
createFamily(astNode *parent, ...)
{
    unsigned int split_size;
    va_list args;
    plAstMaxSplitNode *splitter = (plAstMaxSplitNode *)parent;

    split_size = plAstSplitSize(parent->node_type);
    va_start(args, parent);
    for (unsigned int k=0; k<split_size; k++) {
        plAstNode *node;

        node = va_arg(args, plAstNode*);
        splitter->nodes[k] = node;
#ifdef AST_HAS_PARENT
        if ( node ) {
            node->parent = parent;
        }
#endif
    }
    va_end(args);
}

static int
parseImportExport(plLexicalScanner *scanner, plAstNode **node)
{
    plLexicalMarker_t marker;
    plLexicalToken token;
    plAstNode *node;

    marker = scanner->last_marker;

    if ( TERMINAL_LMARKER(plTokenRead(scanner, &token)) ) {
        return translateTerminalMarker(scanner->last_marker);
    }

    if ( token.marker != PL_LMARKER_NAME ) {
        PARSER_ERROR("Expected name after %s", (marker == PL_LMARKER_IMPORT)? "import" : "export");
        plTokenCleanup(&token);
        return PL_RET_BAD_DATA;
    }

    *node = plAstNew(marker, token.line_no, token.ctx.name);
    if ( !*node ) {
        return PL_RET_OUT_OF_MEMORY;
    }

    return PL_RET_OK;
}

static int
parseFunction(plLexicalScanner *scanner, plAstNode **tree, plLexicalMarker_t marker, bool allow_anonymous)
{
    int ret = PL_RET_BAD_DATA;
    unsigned int starting_line_no;
    const char *name = NULL;
    plLexicalToken token;
    plAstNode *arg_list = NULL;

    starting_line_no = scanner->line_no;

    if ( TERMINAL_LMARKER(plTokenRead(scanner, &token)) ) {
        return translateTerminalMarker(scanner->last_marker);
    }

    if ( token.marker == PL_LMARKER_NAME ) {
        name = token.ctx.name;

        if ( TERMINAL_LMARKER(plTokenRead(scanner, &token)) ) {
            goto error;
        }
    }
    else if ( !allow_anonymous ) {
        PARSER_ERROR("Anonymous %s not allowed in this context", plLexicalMarkerName(marker));
        plTokenCleanup(&token);
        return PL_RET_BAD_DATA;
    }

    if ( TERMINAL_LMARKER(plTokenRead(scanner, &token)) ) {
        goto error;
    }

    if ( token.marker != PL_LMARKER_LEFT_PARENS ) {
        PARSER_ERROR("Expected '(' before argument list");
        plTokenCleanup(&token);
        goto error;
    }

    while ( true ) {
        
    }

error:

    plUnregisterName(scanner->table, name);
    plAstFree(arg_list);

    if ( TERMINAL_LMARKER(scanner->last_marker) ) {
        return translateTerminalMarker(scanner->last_marker);
    }
    else {
        return ret;
    }
}

static int
parseGlobalSpace(plLexicalScanner *scanner, plAstNode **tree)
{
    int ret;
    plLexicalToken token;

    while ( !TERMINAL_LMARKER(plTokenRead(scanner, &token)) ) {
        switch ( scanner->last_marker ) {
        case PL_LMARKER_IMPORT:
        case PL_LMARKER_EXPORT:
            ret = parseImportExport(scanner, tree, scanner->last_marker);
            break;

        case PL_LMARKER_SOURCE:
        case PL_LMARKER_PIPE:
        case PL_LMARKER_SINK:
            ret = parseFunction(scanner, tree, scanner->last_marker, false);
            break;

        case PL_LMARKER_MAIN:
            ret = parseMain(scanner, tree);
            break;

        default:
            PARSER_ERROR("Invalid token in global namespace: %s", plLexicalMarkerName(token.marker));
            return PL_RET_BAD_DATA;
        }

        if ( ret != PL_RET_OK ) {
            return ret;
        }
    }

    ret = translateTerminalMarker(scanner->last_marker);
    if ( ret == PL_RET_OK && !tree ) {
        PARSER_ERROR("File is empty");
        return PL_RET_BAD_DATA;
    }
    return ret;
}

int
plFileParse(FILE *in, const char *file_name, plAstNode **tree, plNameTable **table)
{
    int ret;
    plLexicalScanner scanner;

    if ( !in || !tree || !table ) {
        VASQ_ERROR("in, tree, and table cannot be NULL");
    }

    *tree = NULL;

    *table = plNameTableNew();
    if ( !*table ) {
        return PL_RET_OUT_OF_MEMORY;
    }

    plScannerInit(&scanner, in, file_name, *table);
    VASQ_INFO("Parsing %s", scanner.file_name);

    ret = parseGlobalSpace(&scanner, tree);
    if ( ret != PL_RET_OK ) {
        plAstFree(*tree);
        plNameTableFree(*table);
    }
    return ret;
}
