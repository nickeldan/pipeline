#include <stdlib.h>

#include "dll.h"
#include "plError.h"

dllNode *createNode(int marker, void *data) {
	dllNode *new;

	new=malloc(sizeof(dllNode));
	if ( new ) {
		new->marker=marker;
		new->data=data;
	}

	return new;
}

int insertLeft(int marker, void *data, dll *list, dllNode *here) {
	dll insertMe;

	insertMe.head=insertMe.tail=createNode(marker,data);
	if ( !insertMe.head ) {
		return PL_ERROR_OUT_OF_MEMORY;
	}

	insertListLeft(&insertMe,list,here);

	return PL_ERROR_OK;
}

void insertListLeft(dll *insertMe, dll *list, dllNode *here) {
	if ( !insertMe->head ) {
		return;
	}

	if ( !list->head ) {
		list->head=insertMe->head;
		list->tail=insertMe->tail;

		goto done;
	}

	insertMe->head->prev=here->prev;
	insertMe->tail->next=here;

	if ( here->prev ) {
		here->prev->next=insertMe->head;
	}

	here->prev=insertMe->tail;

	if ( list->head == here ) {
		list->head=insertMe->head;
	}

	done:

	insertMe->head=NULL;
	insertMe->tail=NULL;
}

int insertRight(int marker, void *data, dll *list, dllNode *here) {
	dll insertMe;

	insertMe.head=insertMe.tail=createNode(marker,data);
	if ( !insertMe.head ) {
		return PL_ERROR_OUT_OF_MEMORY;
	}

	insertListRight(&insertMe,list,here);

	return PL_ERROR_OK;
}

void insertListRight(dll *insertMe, dll *list, dllNode *here) {
	if ( !insertMe.head ) {
		return;
	}

	if ( !list->head ) {
		insertMe->head=list->head;
		insertMe->tail=list->tail;

		goto done;
	}

	insertMe->tail->next=here->next;
	insertMe->head->prev=here;

	if ( here->next ) {
		here->next->prev=insertMe->tail;
	}

	here->next=insertMe->head;

	if ( list->tail == here ) {
		list->tail=insertMe->tail;
	}

	done:

	insertMe->head=NULL;
	insertMe->tail=NULL;
}

void deleteNode(dll *list, dllNode *here) {
	if ( here->next ) {
		here->next->prev=here->prev;
	}

	if ( here->prev ) {
		here->prev->next=here->next;
	}

	if ( list->head == here ) {
		list->head=list->head->next;
	}

	if ( list->tail == here ) {
		list->tail=list->tail->prev;
	}

	free(here);
}
