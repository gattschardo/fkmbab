/* shell.c
 *
 * (c) fkmsft, 2010
 */

#include <stdio.h>
#include <ctype.h>

#include "damen.h"

#define PROMPT ("NQueens> ")

int main(int argc, char **argv)
{
	int input, n;

	fputs(PROMPT, stdout);
	while((input = getchar()) != EOF) {
		n = 0;
		while (isdigit(input)) {
			n *= 10;
			n += input - '0';
			input = getchar();
		}
		printf("Got %d\n", n);
		if (n > 0)
			puts(solve(n));
		else 
			puts("Please enter an integer > 0, press ^D to exit");

		fputs(PROMPT, stdout);
	}
	fputc('\n', stdout);
}
