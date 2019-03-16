/* damen.c
 *
 * (c) fkmsft, 2010
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "damen.h"

int main(int argc, char **argv)
{
	int n = 8;
	if (argc > 1)
		n = atoi(argv[1]);

	char *answer = solve(n);
	puts(answer);

        if (strcmp(answer, FAIL))
          free(answer);

	return 0;
}
