#include <sys/types.h>

typedef struct dllNode {
	struct dllNode *prev;
	struct dllNode *next;
	void *data;
	int marker;
} dllNode;

typedef struct dll {
	dllNode *head;
	dllNode *tail;
	size_t length;
} dll;

#define EMPTY_DLL_LITERAL {.head=NULL, .tail=NULL, .length=0}

dllNode *createNode(int marker, void *data);

int insertLeft(int marker, void *data, dll *list, dllNode *here);

void insertListList(dll *insertMe, dll *list, dllNode *here);

int insertRight(int marker, void *data, dll *list, dllNode *here);

void insertRightList(dll *insertMe, dll *list, dllNode *here);

void deleteNode(dll *list, dllNode *here);

void deleteList(dll *list);