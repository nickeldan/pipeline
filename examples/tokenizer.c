#include "../lexer/token.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv) {
	if ( argc < 2 ) {
		printf("Usage: %s filename.pipl\n", argv[0]);
		return 1;
	}
	int fd;
	struct stat f;
	char *text;
	plToken token;

	if ( stat(argv[1],&f) != 0 ) {
		perror("stat");
		return 2;
	}
	text=malloc(f.st_size);
	if ( !text ) {
		perror("malloc");
		return 3;
	}
	fd=open(argv[1],O_RDONLY);
	if ( fd == -1 ) {
		perror("open");
		free(text);
		return 2;
	}
	read(fd,text,f.st_size);
	close(fd);

	grabNextToken(NULL,NULL);
	do {
		grabNextToken(text,&token);
		if ( token.marker == PL_MARKER_NAME || token.marker == PL_MARKER_INVALID_LITERAL ) {
			free(token.value.name);
		}
	} while ( token.marker > PL_MARKER_EOF );
	free(text);

	return 0;
}
