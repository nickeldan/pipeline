#ifndef __PIPELINE_NAME_TABLE_H__
#define __PIPELINE_NAME_TABLE_H__

void
nameTableSetup(void);

char *
registerName(const char *name);

void
removeReference(const char *name);

#endif  // __PIPELINE_NAME_TABLE_H__
