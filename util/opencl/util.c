#include "util.h"

char *myLoadProgSource(const char *filename, size_t *len)
{
	FILE *f = fopen(filename, "r");
	if (!f)
		return 0;

	/* find length of file */
	fseek(f, 0, SEEK_END);
	*len = ftell(f);
	rewind(f);

	char *data = malloc(sizeof(char) * *len);
	*len += fread(data, sizeof(char), *len, f);

	return data;
}
