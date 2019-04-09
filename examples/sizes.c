#include "../pipeline/pipeline.h"

#include <stdio.h>

int main() {
	printf("plObj_value_t has a size of %lu bytes.\n", sizeof(plObj_value_t));
	printf("plObject has a size of %lu bytes.\n", sizeof(plObject));

	return 0;
}