#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "util.h"

void errorQuit(const char *functionName, const char *format, ...) {
	va_list args;

	fprintf(stderr,"%s: ", functionName);

	va_start(args,format);
	vfprintf(stderr,format,args);
	va_end(args);
	fprintf(stderr,"\n");

	exit(1);
}