/* patchboard.c */

#include "enigma/reflector.h"

/*
Reflector *reflector_create(void)
{
	Reflector *r = malloc(sizeof(Reflector));

	r->configuration = string_to_int(alphabet, DEFPATCHBOARD);

	return r;
}
*/

Reflector *reflector_create(const int *configuration)
{
	Reflector *r = malloc(sizeof(Reflector));

	r->configuration = configuration;

	return r;
}

void reflector_delete(Reflector *r)
{
	free((void *)r->configuration);
	free(r);

	return;
}

int reflector_encode(const Reflector *r, int letter)
{
	/*
	if (letter < 0 || letter >= strlen(alphabet))
		foo;
	*/

	return r->configuration[letter];
}

/*
int reflector_decode(const Reflector *r, int letter)
{
	int index = 0;

	while(r->configuration[index] != letter)
		index++;

	return index;
}
*/
