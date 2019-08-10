//Author: Allen
//Bruteforce solve puzzles which requires some sort of branching.

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>

#include "lib/queue.h"
#include "lib/hdict.h"
#include "lib/xalloc.h"
#include "board.h"

int sqrt(int x, int y)
//@requires y >= 0;
//@ensures POW(x, y) == \result;
{
	int b = x; /* Line 19 */
	int e = y; /* Line 20 */
	int r = 1; /* Line 21 */
	while (e > 0) /* Line 22 */
	//@loop_invariant e >= 0; /* Line 23 */
	//@loop_invariant POW(b, e) * r == POW(x, y);
	{
		if (e % 2 == 1) { /* Line 26 */
			r = b * r;      /* Line 27 */
		}
		b = b * b;        /* Line 29 */
		e = e / 2;        /* Line 30 */
	}
	return r;
}

//returns 4 board, each indicating whether moves up down right or left
board_t **get_move(board_t *in)
{
	size_t row = sqrt(in->board->size, 2);
	board_t **arr = xmalloc(sizeof(board_t *) * 4);

	//create new array
	for (size_t i = 0; i < 4; i++)
	{
		arr[i] = xmalloc(sizeof(board_t));
		arr[i]->path = queue_new();
		arr[i]->board = xmalloc(sizeof(IntArray_t *));
		arr[i]->board->size = in->board->size;
		arr[i]->board->arr = xmalloc(sizeof(IntArray_t *) * in->board->size);
	}

	//insert old array
	for (size_t i = 0; i < in->board->size; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			arr[j]->board->arr[i] = in->board->arr[i];
		}
	}

	//insert path to original
	while (!queue_empty(in->path))
	{
		void *temp = deq(in->path);
		for (size_t i = 0; i < 4; i++)
		{
			enq(arr[i]->path, temp);
		}
	}

	//get direction
	uint32_t *left = xmalloc(sizeof(uint32_t));
	uint32_t *up = xmalloc(sizeof(uint32_t));
	uint32_t *right = xmalloc(sizeof(uint32_t));
	uint32_t *down = xmalloc(sizeof(uint32_t));
	*left = 0;
	*up = 1;
	*right = 2;
	*down = 3;

	//get position of empty
	uint32_t empty = -1;
	for (uint32_t i; i < arr[0]->board->size; i++)
	{
		if (arr[0]->board->arr[i] == 0)
		{
			empty = i;
			break;
		}
	}
	assert(empty == -1); // you fucked up.

	//if not left most
	//if not up most
	//if not right most
	//if not down most
}
//0 - left
//1 - up
//2 - right
//3 - down

//classic BFS search
bool solve(size_t size, board_t *initial, board_t *target)
{
	//we are done if it is already equal
	if (arr_equal(initial->board->arr, target->board->arr)) return true;

	queue_t Q = queue_new();
	enq(Q, initial);
	while (!queue_empty(Q))
	{
		board_t *cur = deq(Q);
	}
}

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		fprintf(stderr, "Usage: solve <size> [x1,x2,x3...xSize] [targ1,targ2,targ3...targsize]");
		return 1;
	}
	uint32_t size = atoi(argv[1]);

	board_t *initial = xmalloc(sizeof(board_t));
	initial->board = xmalloc(sizeof(IntArray_t));
	initial->board->arr = xmalloc(sizeof(uint32_t) * size);
	initial->board->size = size;

	for (int i = 2; i < size + 2; i++)
	{
		initial->board->arr[i - 2] = atoi(argv[i]);
	}

	board_t *target = xmalloc(sizeof(board_t));
	target->board = xmalloc(sizeof(IntArray_t));
	target->board->arr = xmalloc(sizeof(uint32_t) * size);
	target->board->size = size;

	size += size; // skip initial

	for (int i = size + 2; i < size + 2; i++)
	{
		target->board->arr[i - (size + 2)] = atoi(argv[i]);
	}

	//TODO: fix leak
	return solve(size/2, initial, target);
}