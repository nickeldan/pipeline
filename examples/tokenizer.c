#include "../lexer/token.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
	if ( argc < 2 ) {
		fprintf(stderr,"Usage: %s filename.pipl\n", argv[0]);
		return 1;
	}

	plFileReader reader;
	plToken token;
	plToken_marker lastMarker;
	off_t lineNo;

	if ( !initReader(&reader,argv[1]) ) {
		return 2;
	}

	lineNo=0;
	lastMarker=PL_MARKER_WHITESPACE;

	do {
		grabNextToken(&reader,&token);
		if ( token.marker == PL_MARKER_NAME || token.marker == PL_MARKER_INVALID_LITERAL || token.marker == PL_MARKER_NAME_TOO_LONG ) {
			free(token.value.name);
		}
		else if ( lastMarker == PL_MARKER_WHITESPACE && token.marker == PL_MARKER_WHITESPACE ) {
			continue;
		}

		if ( token.lineNo > lineNo ) {
			if ( lineNo != 0 ) {
				printf("\n");
			}
			lineNo=token.lineNo;
		}
		printf("%s ", tokenName(&token));

		lastMarker=token.marker;
	} while ( token.marker != PL_MARKER_EOF );
	printf("\n");
	closeReader(&reader);

	return 0;
}
