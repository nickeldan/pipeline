#include <stdlib.h>
#include <string.h>

#include "codeBlock.h"

void freeCodeBlock(codeBlock *block) {
	free(block->codes);
	memset(block,0,sizeof(codeBlock));
}
