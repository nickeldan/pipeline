#include <string.h>

#include "parserInternal.h"

int
plParseStructDefinition(plLexicalScanner *scanner, plAstNode **node)
{
    int ret;
    plLexicalLocation location;
    plLexicalToken token;
    plAstNode *struct_name_node, *arg_list = NULL;

    if (LIKELY(node)) {
        *node = NULL;
    }
    if (UNLIKELY(!scanner || !node)) {
        VASQ_ERROR(debug_logger, "The arguments cannot be NULL.");
        return PL_RET_USAGE;
    }

    plGetLastLocation(scanner, &location);

    ret = NEXT_TOKEN(scanner, &token);
    if (ret != PL_RET_OK) {
        return ret;
    }

    if (token.header.marker != PL_MARKER_NAME) {
        PARSER_ERROR("Expected NAME instead of %s following STRUCT.",
                     plLexicalMarkerName(token.header.marker));
        plTokenCleanup(&token, scanner->table);
        return PL_RET_BAD_DATA;
    }
    struct_name_node = plAstNew(PL_MARKER_NAME);
    plAstCopyTokenInfo(struct_name_node, &token);

    ret = EXPECT_MARKER(scanner, PL_MARKER_LEFT_BRACE, NULL);
    if (ret != PL_RET_OK) {
        goto error;
    }

    while (true) {
        plLexicalLocation colon_location;
        plAstNode *name_node, *type_node, *arg_node;

        ret = NEXT_TOKEN(scanner, &token);
        if (ret != PL_RET_OK) {
            goto error;
        }

        if (token.header.marker == PL_MARKER_RIGHT_BRACE) {
            break;
        }

        if (token.header.marker != PL_MARKER_NAME) {
            PARSER_ERROR("Expected NAME at beginning of struct field definition.");
            ret = PL_RET_BAD_DATA;
            goto loop_error;
        }

        ret = EXPECT_MARKER(scanner, PL_MARKER_COLON, &colon_location);
        if (ret != PL_RET_OK) {
            goto loop_error;
        }

        ret = plParseExtendedType(scanner, &type_node);
        if (ret != PL_RET_OK) {
            goto loop_error;
        }

        name_node = plAstNew(PL_MARKER_NAME);
        plAstCopyTokenInfo(name_node, &token);

        arg_node = plAstCreateFamily(PL_MARKER_COLON, name_node, type_node);
        memcpy(&arg_node->header.location, &colon_location, sizeof(colon_location));

        if (arg_list) {
            ret = plAstCreateConnection(PL_MARKER_SEMICOLON, &arg_list, arg_node);
            if (ret != PL_RET_OK) {
                plAstFree(arg_node, scanner->table);
                goto error;
            }
        }
        else {
            arg_list = arg_node;
        }

        ret = EXPECT_MARKER(scanner, PL_MARKER_SEMICOLON, NULL);
        if (ret != PL_RET_OK) {
            goto error;
        }

        continue;

loop_error:

        plTokenCleanup(&token, scanner->table);
        goto error;
    }

    if (!arg_list) {
        PARSER_ERROR("STRUCT definition cannot have zero fields.");
        ret = PL_RET_BAD_DATA;
        goto error;
    }

    *node = plAstCreateFamily(PL_MARKER_STRUCT, struct_name_node, arg_list);
    memcpy(&(*node)->header.location, &location, sizeof(location));

    return PL_RET_OK;

error:

    plAstFree(struct_name_node, scanner->table);
    plAstFree(arg_list, scanner->table);

    return ret;
}
