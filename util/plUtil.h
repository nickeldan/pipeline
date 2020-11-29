#ifndef __PIPELINE_PLUTIL_H__
#define __PIPELINE_PLUTIL_H__

#include <stdarg.h>
#include <string.h>

#include "plError.h"

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#define BZERO(ptr, size) memset(ptr, 0, size)

// For Linux
#define PATH_MAX 4096

void
errorQuit(const char *functionName, const char *format, ...) __attribute__((noreturn));
#define ERROR_QUIT(format, ...) errorQuit(__PRETTY_FUNCTION__, format, ##__VA_ARGS__)

void *
safeMalloc(const char *functionName, size_t size);
#define SAFE_MALLOC(size) safeMalloc(__PRETTY_FUNCTION__, size)

void *
safeCalloc(const char *functionName, size_t nmemb, size_t size);
#define SAFE_CALLOC(nmemb, size) safeCalloc(__PRETTY_FUNCTION__, nmemb, size)

#endif  // __PIPELINE_UTIL_H__
