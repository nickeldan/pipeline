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
	enum plTokenMarker lastMarker=PL_MARKER_WHITESPACE;
	uint32_t lineNo;

	if ( !initReader(&reader,argv[1]) ) {
		return 2;
	}

	lineNo=0;

	do {
		grabNextToken(&reader,&token);

		if ( token.marker == PL_MARKER_NAME || token.marker == PL_MARKER_LITERAL ) {
			free(token.data);
		}
		else if ( token.marker == PL_MARKER_LITERAL ) {
			plFreeObject((plObject*)token.data);
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
	} while ( !TERMINAL_MARKER(reader.lastMarker) );

	printf("\n");
	closeReader(&reader);

	return 0;
}
