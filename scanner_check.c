#include <unistd.h>

#include "vasq/logger.h"

#include "plUtil.h"
#include "scanner.h"

int
main(int argc, char **argv)
{
    int ret;
    unsigned int line_no = 0;
    FILE *f;
    plLexicalScanner scanner;
    plNameTable *table;
    plLexicalToken token;

    ret = VASQ_LOG_INIT(LL_USE, STDERR_FILENO, false);
    if (ret != VASQ_RET_OK) {
        return plTranslateVasqRet(ret);
    }

    if (argc == 1) {
        f = stdin;
    }
    else {
        f = fopen(argv[1], "r");
        if (!f) {
            VASQ_PERROR("fopen", errno);
            return PL_RET_NO_ACCESS;
        }
    }

    table = plNameTableNew();
    if (!table) {
        ret = PL_RET_OUT_OF_MEMORY;
        goto done;
    }

    plScannerInit(&scanner, f, argv[1], table);

    while (!TERMINAL_MARKER(TOKEN_READ(&scanner, &token))) {
        if (scanner.location.line_no > line_no) {
            if (line_no > 0) {
                printf("\n");
            }
            line_no = scanner.location.line_no;
        }
        printf("%s ", plLexicalMarkerName(token.marker));

        if (token.marker == PL_MARKER_OBJECT) {
            plFreeObject(token.ctx.object);
        }
    }

    printf("\n");
    plNameTableFree(table);

    if (scanner.last_marker == PL_MARKER_EOF) {
        ret = PL_RET_OK;
    }
    else {
        ret = plTranslateTerminalMarker(scanner.last_marker);
    }

done:

    if (argc > 1) {
        fclose(f);
    }

    return ret;
}
