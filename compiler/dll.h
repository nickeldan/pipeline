typedef struct dllNode {
	struct dllNode *prev;
	struct dllNode *next;
	void *data;
	int marker;
} dllNode;

typedef struct dll {
	dllNode *head;
	dllNode *tail;
} dll;

dllNode *createNode(int marker, void *data);

int insertLeft(int marker, void *data, dll *list, dllNode *here);

int insertRight(int marker, void *data, dll *list, dllNode *here);

void deleteNode(dll *list, dllNode *here);
