#include "util.h"

int main(int argc, char **argv)
{
	char *file = "test.c";
	if (argc == 2)
		file = argv[1];
	size_t len = 0;
	char *data = myLoadProgSource(file, &len);
	printf("read %d bytes from %s:\n", len, file);
	puts(data);
	return 0;
}
