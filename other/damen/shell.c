/* shell.c
 *
 * (c) fkmsft, 2010
 */

#include <stdio.h>
#include <ctype.h>

#include "damen.h"
#include "warn.h"

static char const *const prompt = "NQueens> ";

int main(void)
{
	int input = EOF;

	fputs(prompt, stdout);
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
			else {
				puts(fail);
			}
		}
		else {
			puts("Please enter an integer > 0, press ^D to exit");
		}

		fputs(prompt, stdout);
	}
	fputc('\n', stdout);
}
