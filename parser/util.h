#ifndef __PIPELINE_UTIL_H__
#define __PIPELINE_UTIL_H__

#include <string.h>
#include <stdarg.h>

#ifndef MIN
#define MIN(a,b) (((a) < (b))? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b) (((a) > (b))? (a) : (b))
#endif

#define BZERO(ptr,size) memset(ptr,0,size)

void errorQuit(const char *functionName, const char *format, ...) __attribute__((noreturn));
#define ERROR_QUIT(format,...) errorQuit(__PRETTY_FUNCTION__,format,##__VA_ARGS__)

#endif // __PIPELINE_UTIL_H__