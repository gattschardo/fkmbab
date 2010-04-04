/* patchboard.c */

#include "patchboard.h"

/*
Patchboard *patchboard_create(void)
{
	Patchboard *p = malloc(sizeof(Patchboard));

	p->configuration = string_to_int(alphabet, DEFPATCHBOARD);

	return p;
}
*/

Patchboard *patchboard_create(const int *configuration)
{
	Patchboard *p = malloc(sizeof(Patchboard));

	p->configuration = configuration;

	return p;
}

void patchboard_delete(Patchboard *p)
{
	free((void *)p->configuration);
	free(p);

	return;
}

int patchboard_encode(const Patchboard *p, int letter)
{
	/*
	if (letter < 0 || letter >= strlen(alphabet))
		foo;
	*/

	return p->configuration[letter];
}

int patchboard_decode(const Patchboard *p, int letter)
{
	int index = 0;

	while(p->configuration[index] != letter)
		index++;

	return index;
}
