#include "../parser/token.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
	plFileReader reader;
	uint32_t lineNo;

	if ( argc < 2 ) {
		fprintf(stderr,"Usage: %s filename.pipl\n", argv[0]);
		return 1;
	}

	if ( !initReader(&reader,argv[1]) ) {
		return 2;
	}

	lineNo=0;

	do {
		plToken token;

		grabNextToken(&reader,&token);

		if ( token.lineNo > lineNo ) {
			if ( lineNo != 0 ) {
				printf("\n");
			}
			lineNo=token.lineNo;
		}

		printf("%s ", tokenName(&token));

		clearToken(&token);
	} while ( !TERMINAL_MARKER(reader.lastMarker) );

	printf("\n");
	closeReader(&reader);

	return 0;
}
