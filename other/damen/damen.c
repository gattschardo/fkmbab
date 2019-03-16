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

static board init_board(int n);
static bool is_valid(board const *b);
static char *format(board const *b);

static bool add_queen(board *b, int pos);
static bool remove_queen(board *b);
static bool push_queen(board *b);

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

char *solve(int n)
{
	board b = init_board(n);

	bool hope_left = true;
	while (b.last + 1 < b.n && hope_left) {
		// add new queen
		add_queen(&b, 0);

		// push it forward until it stands ok or it hits the bottom of the board
		while (!is_valid(&b) && push_queen(&b))
			;

		while (!is_valid(&b))
			while (!push_queen(&b))
				if (!remove_queen(&b))
					hope_left = false;
	}

	char *solution;
	if (hope_left) {
		solution = format(&b);
	}
	else
		solution = FAIL;

	return solution;
}

static bool is_valid(board const *b)
{
	int i;
	for (i = 0; i <= b->last; i++) {
		int j;

		// Zeilen
		for (j = i + 1; j <= b->last; j++)
			if (b->board[i] == b->board[j]) // zeile rechts
				return false;

		// Diagonalen
		for (j = 1; j + i <= b->last; j++) {
			if (b->board[i + j] == b->board[i] + j) // dia unten rechts
				return false;
		    if (b->board[i + j] == b->board[i] - j) // dia oben rechts
				return false;
		}
	}

	return true;
}

static bool add_queen(board *b, int pos)
{
	if (++(b->last) < b->n) {
		b->board[b->last] = pos;
		return true;
	} else
		return false;
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
		return false;
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
