#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void)
{
	int col;
	char str[8];

	srand(time(0));

	col = rand() % 0x1000000;
	col = col < 0 ? -col : col;

	snprintf(str, 8, "#%06x", col);

	execlp("/usr/bin/urxvt", "urxvt", "-fg", str, "-tint", str, 0);

	return 1;
}
