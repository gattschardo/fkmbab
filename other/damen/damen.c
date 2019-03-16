/* util.c
 *
 * (c) fkmsft, 2010
 */

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "damen.h"
#include "warn.h"

#define QUEEN '*'
#define EMPTY '.'

typedef struct {
	int n;
	int last;
	int *board;
} board;

static NODISCARD board init_board(int n);
static NODISCARD bool solve_board(board *b);
static NODISCARD bool is_valid(board const *b);
static NODISCARD char *format(board const *b);
static void destroy_board(board const *b);

static void add_queen(board *b);
static NODISCARD bool remove_queen(board *b);
static NODISCARD bool push_queen(board *b);

static board init_board(int n)
{
	assert(n >= 0);

	board b;
	b.n = n;
	b.last = -1;
	b.board = (int*)malloc((size_t)b.n*sizeof(int));

	int i;
	for (i = 0; i < b.n; i++)
		b.board[i] = -1;

	return b;
}

static void destroy_board(board const *b)
{
	free(b->board);
}

char *solve(int n)
{
	board b = init_board(n);

	char *solution = solve_board(&b) ? format(&b) : FAIL;

	destroy_board(&b);

	return solution;
}

static bool solve_board(board *b)
{
	while (b->last + 1 < b->n) {
		// add new queen
		add_queen(b);

		// push it forward until it stands ok or it hits the bottom of the board
		while (!is_valid(b) && push_queen(b))
			;

		while (!is_valid(b))
			while (!push_queen(b))
				if (!remove_queen(b)) {
					return false;
				}
	}
	return true;
}

static bool is_valid(board const *b)
{
	int i;
	for (i = 0; i <= b->last; i++) {
		int j;

		// rows
		for (j = i + 1; j <= b->last; j++)
			// row to the right
			if (b->board[i] == b->board[j])
				return false;

		// diagonals
		for (j = 1; j + i <= b->last; j++) {
			// diagonal below/right
			if (b->board[i + j] == b->board[i] + j)
				return false;

			// diagonal above/right
			if (b->board[i + j] == b->board[i] - j)
				return false;
		}
	}

	return true;
}

static void add_queen(board *b)
{
	++b->last;
	assert(b->last < b->n);
	b->board[b->last] = 0;
}

static bool remove_queen(board *b)
{
	if (b->last < 1) {
		b->last--;
		return false;
	}
	else {
		b->board[b->last] = -1;
		b->last--;
		return true;
	}
}

static bool push_queen(board *b)
{
	if (b->board[b->last]+1 < b->n) {
		b->board[b->last]++;
		return true;
	}
	else
	{
		return false;
	}
}

static char *format(board const *b)
{
	char *ptr, *solution = (char*)calloc(sizeof(char),(size_t)b->n*(size_t)(b->n+1)+1);
	int i,j;
	ptr = solution;

	for (i = 0; i < b->n; i++) {
		for (j = 0; j < b->n; j++)
			*(ptr++) = (b->board[i] == j)? QUEEN : EMPTY;
		*(ptr++) = '\n';
	}
	*ptr = '\0';

	return solution;
}
