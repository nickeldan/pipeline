#include "../pipeline/object.h"
#include "../parser/token.h"

#include <stdio.h>

int main() {
	printf("plObject: %lu\n", sizeof(plObject));
	printf("plObjectNumber: %lu\n", sizeof(plObjectNumber));
	printf("plObjectByteArray: %lu\n", sizeof(plObjectByteArray));
	printf("plObjectArray: %lu\n", sizeof(plObjectArray));
	printf("plObjectStruct: %lu\n", sizeof(plObjectStruct));
	printf("plToken: %lu\n", sizeof(plToken));

	return 0;
}