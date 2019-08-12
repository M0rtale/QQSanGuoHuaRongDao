//Author: Allen
//Bruteforce solve puzzles which requires some sort of branching.

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

#include "lib/queue.h"
#include "lib/hdict.h"
#include "lib/xalloc.h"
#include "board.h"

int sqrt_i(int x)
//@requires y >= 0;
//@ensures POW(x, y) == \result;
{
	float sqrt = 0;
	float temp = 0;

	// store the half of the given number e.g from 256 => 128
	sqrt = (float)x / 2;
	temp = 0;

	// Iterate until sqrt is different of temp, that is updated on the loop
	while (sqrt != temp) {
		// initially 0, is updated with the initial value of 128
		// (on second iteration = 65)
		// and so on
		temp = sqrt;

		// Then, replace values (256 / 128 + 128 ) / 2 = 65
		// (on second iteration 34.46923076923077)
		// and so on
		sqrt = (x / temp + temp) / 2;
	}
	return (int)sqrt;
}

void swap(uint32_t *one, uint32_t *two)
{
	uint32_t temp = *one;
	*one = *two;
	*two = temp;
}

void printBoard(board_t *b)
{
	printf("\n");

	for (int i = 0; i < sqrt_i(b->board->size); i++)
	{

	} 
}

//returns 4 board, each indicating whether moves up down right or left
board_t **get_move(board_t *in)
{
	size_t row = sqrt_i(in->board->size);
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
	uint32_t empty = (uint32_t)-1;
	for (uint32_t i = 0; i < arr[0]->board->size; i++)
	{
		//printf("%d", arr[0]->board->arr[i]);
		if (arr[0]->board->arr[i] == 0)
		{
			empty = i;
			break;
		}
	}
	//printf("\n%d\n", empty);
	assert(empty != (uint32_t)-1); // you fucked up.

	//if not left most
	// TODO: fix this shit, is it 0 or 1.
	if (empty % row - 1 > 0)
	{
		swap(&arr[0]->board->arr[empty], &arr[0]->board->arr[empty - 1]);
		enq(arr[0]->path, left);
	}
	//if not up most
	if (empty / row - 1 > 0)
	{
		swap(&arr[1]->board->arr[empty], &arr[1]->board->arr[empty - row]);
		enq(arr[1]->path, up);
	}
	//if not right most
	if(empty % row + 1 <= row)
	{
		swap(&arr[2]->board->arr[empty], &arr[2]->board->arr[empty + 1]);
		enq(arr[2]->path, right);
	}
	//if not down most
	if (empty / row + 1 <= row)
	{
		swap(&arr[3]->board->arr[empty], &arr[3]->board->arr[empty + row]);
		enq(arr[3]->path, down);
	}
	return arr;
}
//0 - left
//1 - up
//2 - right
//3 - down

//classic BFS search
bool solve(board_t *initial, board_t *target)
{
	//we are done if it is already equal
	if (arr_equal(initial->board, target->board)) return true;

	queue_t Q = queue_new();
	enq(Q, initial);

	hdict_t dict = ht_new(initial->board->size);
	hdict_insert(dict, initial);
	while (!queue_empty(Q))
	{
		board_t *cur = deq(Q);
		board_t **move = get_move(cur);


		for (int i = 0; i < 4; i++)
		{
			//if success in reproducing the path
			if (arr_equal(move[i]->board, target->board))
			{
				queue_t path = move[i]->path;
				while (!queue_empty(path))
				{
					uint32_t paths = *(uint32_t *)deq(path);
					switch (paths)
					{
					case 0: // left
					{
						printf("l ");
						break;
					}
					case 1: //up
					{
						printf("u ");
						break;
					}
					case 2: //right
					{
						printf("r ");
						break;
					}
					case 3: //down
					{
						printf("d ");
						break;
					}
					}
				}
				printf("\n");
				
				return true;
			}
			//if the thing is bad
			if (ht_lookup(dict, move[i]->board) == NULL)
			{
				enq(Q, move[i]);
				hdict_insert(dict, move[i]);
			}
		}
	}
	//no path could be found.
	return false;
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
	initial->path = queue_new();

	for (size_t i = 2; i < size + 2; i++)
	{
		initial->board->arr[i - 2] = atoi(argv[i]);
		printf("%d ", atoi(argv[i]));
	}

	printf("\n");

	board_t *target = xmalloc(sizeof(board_t));
	target->board = xmalloc(sizeof(IntArray_t));
	target->board->arr = xmalloc(sizeof(uint32_t) * size);
	target->board->size = size;

	for (size_t i = size + 2; i < size + size + 2; i++)
	{
		target->board->arr[i - (size + 2)] = atoi(argv[i]);
		printf("%d ", atoi(argv[i]));
	}

	printf("\n");

	//TODO: fix leak
	if (solve(initial, target))
	{
		printf("Success!\n");
		return 0;
	}
	else
	{
		printf("err...\n");
		return 1;
	}
}