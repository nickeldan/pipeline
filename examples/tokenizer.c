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
	off_t lineNo;

	if ( !initReader(&reader,argv[1]) ) {
		return 2;
	}
	lineNo=0;
	do {
		grabNextToken(&reader,&token);
		if ( token.marker == PL_MARKER_NAME || token.marker == PL_MARKER_INVALID_LITERAL || token.marker == PL_MARKER_NAME_TOO_LONG ) {
			free(token.value.name);
		}
		printf("%s ", tokenName(&token));
		if ( reader.lineNo > lineNo ) {
			printf("\n");
			lineNo=reader.lineNo;
		}
	} while ( token.marker != PL_MARKER_EOF );
	printf("\n");
	closeReader(&reader);

	return 0;
}
