#ifndef __PIPELINE_CODE_BLOCK_H__
#define __PIPELINE_CODE_BLOCK_H__

#include <sys/types.h>

typedef struct byteCode {

} byteCode;

typedef struct codeBlock {
	byteCode *codes;
	size_t capacity;
	size_t length;
} codeBlock;

void freeCodeBlock(codeBlock *block);

#endif // __PIPELINE_CODE_BLOCK_H__
