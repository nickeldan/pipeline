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
	dllNode *new;

	new=createNode(marker,data);
	if ( !new ) {
		return PL_ERROR_OUT_OF_MEMORY;
	}

	if ( !list->head ) {
		new->prev=new->next=NULL;
		list->head=list->tail=new;

		return PL_ERROR_OK;
	}

	new->prev=here->prev;
	new->next=here;

	if ( here->prev ) {
		here->prev->next=new;
	}

	here->prev=new;

	if ( list->head == here ) {
		list->head=new;
	}

	return PL_ERROR_OK;
}

int insertRight(int marker, void *data, dll *list, dllNode *here) {
	dllNode *new;

	new=createNode(marker,data);
	if ( !new ) {
		return PL_ERROR_OUT_OF_MEMORY;
	}

	if ( !list->head ) {
		new->prev=new->next=NULL;
		list->head=list->tail=new;

		return PL_ERROR_OK;
	}

	new->next=here->next;
	new->prev=here;

	if ( here->next ) {
		here->next->prev=new;
	}

	here->next=new;

	if ( list->tail == here ) {
		list->tail=new;
	}

	return PL_ERROR_OK;
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
