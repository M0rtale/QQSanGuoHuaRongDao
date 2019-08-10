//wrapper implementation

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "board.h"
#include "lib/hdict.h"
#include "lib/queue.h"
#include "lib/xalloc.h"

//converts type board* to type IntArray*
hdict_key entry_key_fn(hdict_entry e)
{
	return ((board_t *)e)->board;
}

//compare all value inside of array
bool key_equal_n(hdict_key a1, hdict_key a2)
{
	IntArray_t *temp1 = (IntArray_t *)a1;
	IntArray_t *temp2 = (IntArray_t *)a2;
	for (size_t i = 0; i < temp1->size; i++)
	{
		if (temp1->arr[i] != temp2->arr[i]) return false;
	}
	return true;
}

//hash by ^
size_t hash_key_n(hdict_key k)
{
	size_t hash = 1;
	IntArray_t *temp1 = (IntArray_t *)k;
	//shitty hashing function, but works fine, so gogogo
	for (size_t i = 0; i < temp1->size; i++)
	{
		size_t temp = temp1->arr[i];
		temp = temp ^ i;
		hash = temp * hash;
	}
	return hash;
}

//deconstructor
void entry_free_fn(hdict_entry a1)
{
	board_t *board = (board_t *)a1;
	free(board->board->arr);
	free(board->board);
	queue_free(board->path, &free);
	free(board);
}

hdict_t ht_new(size_t capacity)
{
	assert(capacity > 0);
	hdict_t mew = hdict_new(capacity,
		&entry_key_fn,
		&key_equal_n,
		&hash_key_n,
		&entry_free_fn);
	return mew;
}

board_t *ht_lookup(hdict_t H, IntArray_t *B)
{
	return (board_t *)hdict_lookup(H, B);
}

void ht_insert(hdict_t H, IntArray_t *B)
{
	hdict_insert(H, (void *)B);
}

bool arr_equal(IntArray_t *a1, IntArray_t *a2)
{
	for (int i = 0; i < a1->size; i++)
	{
		if (a1->arr[i] != a2->arr[i]) return false;
	}
	return true;
}