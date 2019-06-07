#include "../pipeline/object.h"
#include "../parser/token.h"

#include <stdio.h>

int main() {
	printf("plObject_t: %lu\n", sizeof(plObject_t));
	printf("plObjectNumber_t: %lu\n", sizeof(plObjectNumber_t));
	printf("plObjectByteArray_t: %lu\n", sizeof(plObjectByteArray_t));
	printf("plObjectArray_t: %lu\n", sizeof(plObjectArray_t));
	printf("plObjectStruct_t: %lu\n", sizeof(plObjectStruct_t));
	printf("plToken_t: %lu\n", sizeof(plToken_t));

	return 0;
}