/* batch.c
 *
 * (c) fkmsft, 2010
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "damen.h"
#include "warn.h"

int main(int argc, char **argv)
{
	int const n = argc > 1 ? atoi(argv[1]) : 8;

	if (n < 0) {
		fprintf(stderr, "negative board size (%d) is not supported\n", n);
		return 1;
	}

	board_t *answer = solve(n);
	if (answer) {
		print_board(stdout, answer);
		destroy_board(answer);
	}
	else {
		puts(fail);
	}

	return 0;
}
