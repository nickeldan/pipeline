#ifndef __PIPELINE_UTIL_H__
#define __PIPELINE_UTIL_H__

#ifndef MAX
#define MAX(a,b) ( ( (a) > (b) )? (a) : (b) )
#endif

#ifndef MIN
#define MIN(a,b) ( ( (a) < (b) )? (a) : (b) )
#endif

typedef unsigned char bool;

#ifndef TRUE
#define TRUE 1
#elif !TRUE
#error TRUE must be true.
#endif

#ifndef FALSE
#define FALSE 0
#elif FALSE
#error FALSE must be false.
#endif

#endif // __PIPELINE_UTIL_H__