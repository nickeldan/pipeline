#include <unistd.h>

#include "vasq/logger.h"

#include "util.h"

#include "scanner.h"

int
main(int argc, char **argv)
{
    int ret;
    unsigned int line_no = 0;
    FILE *f;
    const char *file_name;
    plLexicalScanner scanner;
    plLexicalToken token;

    ret = plSetupDebuggingLogger(LL_USE);
    if (ret != 0) {
        return ret;
    }

    if (argc == 1) {
        f = stdin;
        file_name = "stdin";
    }
    else {
        f = fopen(argv[1], "r");
        if (!f) {
            VASQ_PERROR(debug_logger, "fopen", errno);
            return PL_RET_NO_ACCESS;
        }
        file_name = argv[1];
    }

    ret = plScannerInit(&scanner, f, file_name);
    if (ret != PL_RET_OK) {
        goto done;
    }

    while (!TERMINAL_MARKER(TOKEN_READ(&scanner, &token))) {
        if (scanner.location.line_no > line_no) {
            if (line_no > 0) {
                printf("\n");
            }
            line_no = scanner.location.line_no;
        }
        printf("%s ", plLexicalMarkerName(token.header.marker));

        if (token.header.marker == PL_MARKER_OBJECT) {
            plFreeObject(&token.data.handle);
        }
    }

    printf("\n");

    if (scanner.last_marker == PL_MARKER_EOF) {
        ret = PL_RET_OK;
    }
    else {
        ret = plTranslateTerminalMarker(scanner.last_marker);
    }
    plScannerCleanup(&scanner);

done:

    if (argc > 1) {
        fclose(f);
    }

    return ret;
}
