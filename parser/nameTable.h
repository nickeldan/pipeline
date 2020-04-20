#ifndef __PIPELINE_NAME_TABLE_H__
#define __PIPELINE_NAME_TABLE_H__

#include <sys/types.h>

void nameTableSetup(void);

char *registerName(const char *name);

#endif // __PIPELINE_NAME_TABLE_H__