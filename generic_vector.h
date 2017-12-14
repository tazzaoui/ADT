#ifndef VECTOR_H
#define VECTOR_H
#include "generic.h"
#include "status.h"

typedef void* GENERIC_VECTOR;


GENERIC_VECTOR generic_vector_init_default(Status (*item_assignment)(ITEM*, ITEM),
                                           void (*item_destroy)(ITEM*));

Status generic_vector_push_back(GENERIC_VECTOR hVector, ITEM item);

//Accessor functions
int generic_vector_get_size(GENERIC_VECTOR hVector);
int generic_vector_get_capacity(GENERIC_VECTOR hVector);


Status generic_vector_pop_back(GENERIC_VECTOR hVector);


Boolean generic_vector_empty(GENERIC_VECTOR hVector);

ITEM generic_vector_at(GENERIC_VECTOR hVector, unsigned int index);

void generic_vector_destroy(GENERIC_VECTOR* phVector);

#endif
