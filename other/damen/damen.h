/* damen.h
 *
 * (c) fkmsft, 2010
 */

#ifndef DAMEN_H
#define DAMEN_H

#include <stdbool.h>

#define QUEEN ('*')
#define EMPTY ('.')
#define FAIL ("No solution")

typedef struct {
	int n;
	int last;
	int *board;
} board;

char *solve(unsigned int n);
bool isvalid(board *b);

bool addqueen(board *b, int pos);
bool removequeen(board *b);
bool pushqueen(board *b);
char *tostr(board *b);

#endif /* DAMEN_H */
