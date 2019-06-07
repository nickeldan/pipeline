#include "../parser/token.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
	plFileReader_t reader;
	uint32_t lineNo;

	if ( argc < 2 ) {
		fprintf(stderr,"Usage: %s filename.pipl\n", argv[0]);
		return 1;
	}

	if ( !init_reader(&reader,argv[1]) ) {
		return 2;
	}

	lineNo=0;

	do {
		plToken_t token;

		read_next_token(&reader,&token);

		if ( token.lineNo > lineNo ) {
			if ( lineNo != 0 ) {
				printf("\n");
			}
			lineNo=token.lineNo;
		}

		printf("%s ", marker_name(token.marker)));

		clear_token(&token);
	} while ( !TERMINAL_MARKER(reader.lastMarker) );

	printf("\n");
	close_reader(&reader);

	return 0;
}
