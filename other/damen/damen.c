/* damen.c
 *
 * (c) fkmsft, 2010
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "damen.h"
#include "warn.h"

#define QUEEN '*'
#define EMPTY '.'

struct board_s {
	int n;
	int last;
	int *board;
};

typedef struct board_s board_t;

static NODISCARD board_t *init_board(int n);
static NODISCARD bool solve_board(board_t *b);
static NODISCARD bool is_valid(board_t const *b);

static void add_queen(board_t *b);
static NODISCARD bool remove_queen(board_t *b);
static NODISCARD bool push_queen(board_t *b);

static board_t *init_board(int n)
{
	assert(n >= 0);

	board_t *b = (board_t *)malloc(sizeof(board_t));
	b->n = n;
	b->last = -1;
	size_t l = (size_t)n;
	b->board = (int *)malloc(l * sizeof(int));
	memset(b->board, -1, l);

	return b;
}

void destroy_board(board_t *b)
{
	free(b->board);
	free(b);
}

board_t *solve(int n)
{
	board_t *b = init_board(n);

	if (solve_board(b)) {
		return b;
	}

	destroy_board(b);

	return 0;
}

static bool solve_board(board_t *b)
{
	while (b->last + 1 < b->n) {
		// add new queen
		add_queen(b);

		// push it forward until it stands ok or it hits the bottom of the board
		while (!is_valid(b) && push_queen(b)) {
			;
		}

		while (!is_valid(b)) {
			while (!push_queen(b)) {
				if (!remove_queen(b)) {
					return false;
				}
			}
		}
	}
	return true;
}

static bool is_valid(board_t const *b)
{
	for (int i = 0; i <= b->last; i++) {

		// rows
		for (int j = i + 1; j <= b->last; j++) {
			// row to the right
			if (b->board[i] == b->board[j]) {
				return false;
			}
		}

		// diagonals
		for (int j = 1; j + i <= b->last; j++) {
			// diagonal below/right
			if (b->board[i + j] == b->board[i] + j) {
				return false;
			}

			// diagonal above/right
			if (b->board[i + j] == b->board[i] - j) {
				return false;
			}
		}
	}

	return true;
}

static void add_queen(board_t *b)
{
	++b->last;
	assert(b->last < b->n);
	b->board[b->last] = 0;
}

static bool remove_queen(board_t *b)
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

static bool push_queen(board_t *b)
{
	if (b->board[b->last]+1 < b->n) {
		b->board[b->last]++;
		return true;
	}
	else {
		return false;
	}
}

void print_board(FILE *stream, board_t const *b)
{
	for (int i = 0; i < b->n; i++) {
		for (int j = 0; j < b->n; j++) {
			fputc(b->board[i] == j ? QUEEN : EMPTY, stream);
		}
		fputc('\n', stream);
	}
}
