#include "token.h"

const char *
plLexicalMarkerName(int marker)
{
    switch (marker) {
    case PL_MARKER_ARROW: return "ARROW";
    case PL_MARKER_NAME: return "NAME";
    case PL_MARKER_OBJECT: return "OBJECT";
    case PL_MARKER_TYPE: return "TYPE";
    case PL_MARKER_SOURCE: return "SOURCE";
    case PL_MARKER_PIPE: return "PIPE";
    case PL_MARKER_SINK: return "SINK";
    case PL_MARKER_LOCAL: return "LOCAL";
    case PL_MARKER_STRUCT: return "STRUCT";
    case PL_MARKER_OPAQUE: return "OPAQUE";
    case PL_MARKER_WHILE: return "WHILE";
    case PL_MARKER_IF: return "IF";
    case PL_MARKER_EIF: return "EIF";
    case PL_MARKER_ELSE: return "ELSE";
    case PL_MARKER_PROD: return "PROD";
    case PL_MARKER_DROP: return "DROP";
    case PL_MARKER_END: return "END";
    case PL_MARKER_NOT: return "NOT";
    case PL_MARKER_LOGICAL: return "LOGICAL";
    case PL_MARKER_CONT: return "CONT";
    case PL_MARKER_BREAK: return "BREAK";
    case PL_MARKER_VERIFY: return "VERIFY";
    case PL_MARKER_EXIT: return "EXIT";
    case PL_MARKER_IS: return "IS";
    case PL_MARKER_AS: return "AS";
    case PL_MARKER_IMPORT: return "IMPORT";
    case PL_MARKER_EXPORT: return "EXPORT";
    case PL_MARKER_EXPORT_ALL: return "EXPORT_ALL";
    case PL_MARKER_MAIN: return "MAIN";
    case PL_MARKER_SEMICOLON: return "';'";
    case PL_MARKER_COLON: return "':'";
    case PL_MARKER_COMMA: return "','";
    case PL_MARKER_PERIOD: return "'.'";
    case PL_MARKER_QUESTION: return "'?'";
    case PL_MARKER_UNDERSCORE: return "'_'";
    case PL_MARKER_LEFT_PARENS: return "'('";
    case PL_MARKER_RIGHT_PARENS: return "')'";
    case PL_MARKER_LEFT_BRACKET: return "'['";
    case PL_MARKER_RIGHT_BRACKET: return "']'";
    case PL_MARKER_LEFT_BRACE: return "'{'";
    case PL_MARKER_RIGHT_BRACE: return "'}'";
    case PL_MARKER_ARITHMETIC: return "ARITHMETIC";
    case PL_MARKER_REASSIGNMENT: return "REASSIGNMENT";
    case PL_MARKER_COMPARISON: return "COMPARISON";
    case PL_MARKER_CONTEXT: return "CONTEXT";
    case 'A': return "'A'";
    case '%': return "'%'";
    default: return "INVALID";
    }
}
