/* rotor.c */

#include "enigma/rotor.h"

Rotor *rotor_create(const int *configuration, int tickpos, int position,
		Rotor *next)
{
	Rotor *r = malloc(sizeof(Rotor));

	r->configuration = configuration;
	r->conflen = strlen(alphabet);
	r->tickpos = tickpos;
	r->position = position;
	r->next = next;

	return r;
}

void rotor_delete(Rotor *r)
{
	free(r);

	return;
}

int rotor_encode(const Rotor *r, int letter)
{
	int tmp = r->configuration[(r->position + letter) % r->conflen];
	return (tmp + r->conflen - r->position) % r->conflen;
}

int rotor_decode(const Rotor *r, int letter)
{
	int index = r->position, count = 0;
	int target = (letter + index) % r->conflen;

	while (r->configuration[index] != target) {
		index = (index + 1) % r->conflen;
		count++;
	}

	return count;
}

void rotor_tick(Rotor *r)
{
	if (r->next && r->position == r->tickpos) {
		rotor_tick(r->next);
		if (debug)
			printf("%c: tick\n", r->tickpos);
	}

	r->position = (r->position + 1) % r->conflen;

	return;
}
