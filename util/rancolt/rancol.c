#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void)
{
	srand(time(0));

	int col = rand() % 0x1000000;

	char str[8];
	sprintf(str, "#%06x", col);

	execlp("/usr/bin/urxvt", "urxvt", "-fg", str, "-tint", str, 0);
}
