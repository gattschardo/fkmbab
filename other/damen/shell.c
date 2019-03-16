/* shell.c
 *
 * (c) fkmsft, 2010
 */

#include <stdio.h>
#include <ctype.h>

#include "damen.h"
#include "warn.h"

#define PROMPT "NQueens> "

int main(void)
{
	int input = EOF;

	fputs(PROMPT, stdout);
	while((input = getchar()) != EOF) {
		int n = 0;
		while (isdigit(input)) {
			n *= 10;
			n += input - '0';
			input = getchar();
		}
		if (n > 0) {
			board_t *answer = solve(n);
			if (answer) {
				print_board(stdout, answer);
				destroy_board(answer);
			}
			else
				puts(FAIL);
		}
		else 
			puts("Please enter an integer > 0, press ^D to exit");

		fputs(PROMPT, stdout);
	}
	fputc('\n', stdout);
}
