#include <unistd.h>

#include "vasq/logger.h"

#include "scanner.h"

#ifdef DEBUG
#define LL_USE VASQ_LL_DEBUG
#else
#define LL_USE VASQ_LL_RAWONLY
#endif

int main(int argc, char **argv) {
    int ret;
    unsigned int line_no = 1;
    FILE *f;
    plLexicalScanner scanner;
    plNameTable *table;
    plLexicalToken token;

    if ( VASQ_LOG_INIT(LL_USE, STDERR_FILENO, false) != VASQ_RET_OK ) {
        return 1;
    }

    if ( argc == 1 ) {
        f = stdin;
    }
    else {
        f = fopen(argv[1], "r");
        if ( !f ) {
            VASQ_PERROR("fopen", errno);
            return 2;
        }
    }

    table = plNameTableNew();
    if ( !table ) {
        ret = 3;
        goto done;
    }

    plScannerInit(&scanner, f, argv[1], table);

    while ( !TERMINAL_LMARKER(plTokenRead(&scanner, &token)) ) {
        if ( scanner.line_no > line_no ) {
            printf("\n");
            line_no = scanner.line_no;
        }
        printf("%s ", plLexicalMarkerName(token.marker));

        if ( token.marker == PL_LMARKER_LITERAL ) {
            plFreeObject(token.ctx.object);
        }
    }

    printf("\n");
    plNameTableFree(table);

    if ( scanner.last_marker != PL_LMARKER_EOF ) {
        ret = 4;
    }
    else {
        ret = 0;
    }

done:

    if ( argc > 1 ) {
        fclose(f);
    }

    return ret;
}