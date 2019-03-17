#include "../lexer/token.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
	if ( argc < 2 ) {
		printf("Usage: %s filename.pipl\n", argv[0]);
		return 1;
	}

	plFileReader reader;
	plToken token;

	if ( !initReader(&reader,argv[1]) ) {
		return 2;
	}
	do {
		grabNextToken(&reader,&token);
		if ( token.marker == PL_MARKER_NAME || token.marker == PL_MARKER_INVALID_LITERAL || token.marker == PL_MARKER_NAME_TOO_LONG ) {
			free(token.value.name);
		}
	} while ( GOOD_MARKER(token.marker) );
	closeReader(&reader);

	return 0;
}
