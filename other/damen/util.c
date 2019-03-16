/* util.c
 *
 * (c) fkmsft, 2010
 */

#include <stdlib.h>
#include <assert.h>

#include "damen.h"
#include "warn.h"

char *solve(int n)
{
	assert(n >= 0);
	board *b = (board*)malloc(sizeof(board));
	b->n = n;
	b->last = -1;
	b->board = (int*)malloc((size_t)b->n*sizeof(int));

	int i;
	for (i = 0; i < b->n; i++)
		b->board[i] = -1;

	bool hopeleft = true;
	while (b->last + 1 < b->n && hopeleft) {
		// add new queen
		addqueen(b, 0);

		// push it forward until it stands ok or it hits the bottom of the board
		while (!isvalid(b) && pushqueen(b))
			;

		while (!isvalid(b))
			while (!pushqueen(b))
				if (!removequeen(b))
					hopeleft = false;
	}

	char *solution;
	if (hopeleft) {
		solution = tostr(b);
	}
	else
		solution = FAIL;

	return solution;
}

bool isvalid(board *b)
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

bool addqueen(board *b, int pos)
{
	if (++(b->last) < b->n) {
		b->board[b->last] = pos;
		return true;
	} else
		return false;
}

bool removequeen(board *b)
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

bool pushqueen(board *b)
{
	if (b->board[b->last]+1 < b->n) {
		b->board[b->last]++;
		return true;
	}
	else
		return false;
}

char *tostr(board *b)
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
