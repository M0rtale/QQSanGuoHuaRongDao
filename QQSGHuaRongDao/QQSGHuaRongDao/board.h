// hash table wrapper

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "lib/queue.h"
#include "lib/hdict.h"

#ifndef BORD_H
#define	BORD_H

typedef struct IntArray IntArray_t;
struct IntArray
{
	//stores the array
	uint32_t *arr;
	//stores size;
	size_t size;
};

typedef struct board board_t;
struct board
{
	//stores the current board
	IntArray_t *board;
	//stores the path required to get here
	queue_t path;
};

hdict_t ht_new(size_t capacity);

board_t *ht_lookup(hdict_t H, IntArray_t *key);

void ht_insert(hdict_t H, IntArray_t *key);

bool arr_equal(IntArray_t *a1, IntArray_t *a2);

#endif