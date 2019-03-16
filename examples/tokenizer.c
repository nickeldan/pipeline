#include "../lexer/token.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv) {
	if ( argc < 2 ) {
		printf("Usage: %s filename.pipl\n", argv[0]);
		return 1;
	}
	int fd;
	struct stat f;
	plFileReader reader;
	plToken token;

	if ( stat(argv[1],&f) != 0 ) {
		perror("stat");
		return 2;
	}
	reader.text=malloc(f.st_size+1);
	if ( !reader.text ) {
		perror("malloc");
		return 3;
	}
	fd=open(argv[1],O_RDONLY);
	if ( fd == -1 ) {
		perror("open");
		free(reader.text);
		return 2;
	}
	read(fd,reader.text,f.st_size);
	close(fd);
	reader.text[f.st_size]='\0';
	reader.textLen=f.st_size+1;
	reader.idx=0;
	reader.lineNo=0;

	do {
		grabNextToken(&reader,&token);
		if ( token.marker == PL_MARKER_NAME || token.marker == PL_MARKER_INVALID_LITERAL ) {
			free(token.value.name);
		}
	} while ( GOOD_MARKER(token.marker) );
	free(reader.text);

	return 0;
}
