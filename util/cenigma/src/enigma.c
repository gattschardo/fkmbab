/* enigma.c */

#include "enigma.h"

Enigma *enigma_create(int *patchboardconf, int *reflectorconf, int rotor_count,
		int **rotor_permutations, int *rotor_ticks, int *rotor_stars)
{
	if (!reflectorconf)
		return 0;

	Enigma *e = malloc(sizeof(Enigma));

	e->rotor_array = malloc(sizeof(Rotor*) * rotor_count);
	e->rotor_count = rotor_count;

	e->patchboard = patchboardconf ? patchboard_create(patchboardconf) : 0;
	e->reflector = reflector_create(reflectorconf);

	int i;
	Rotor *prev = 0;
	for (i = rotor_count - 1; i >= 0; i--)
		prev = e->rotor_array[i] = rotor_create(rotor_permutations[i],
				rotor_ticks[i], rotor_stars[i], prev);

	return e;
}

void enigma_delete(Enigma *e)
{
	if (e->patchboard)
		patchboard_delete(e->patchboard);

	reflector_delete(e->reflector);

	int i;
	for (i = 0; i < e->rotor_count; i++)
		rotor_delete(e->rotor_array[i]);

	free(e->rotor_array);
	free(e);

	return;
}

int enigma_encode(Enigma *e, int letter)
{
	int code = letter;

	if (e->patchboard) {
		 code = patchboard_encode(e->patchboard, code);
		if (debug)
			printf("%c->patchboard->%c", int_to_char(alphabet, letter),
					int_to_char(alphabet, code));
	}

	int i;
	for (i = 0; i < e->rotor_count; i++) {
		code = rotor_encode(e->rotor_array[i], code);
		if (debug)
			printf("->rotor[%d]->%c", i, int_to_char(alphabet, code));
	}

	code = reflector_encode(e->reflector, code);
	if (debug)
		printf("->reflector\nreflector->%c", int_to_char(alphabet, code));

	for (i = e->rotor_count - 1; i >= 0; i--) {
		code = rotor_decode(e->rotor_array[i], code);
		if (debug)
			printf("->rotor[%d]->%c", i, int_to_char(alphabet, code));
	}

	if (e->patchboard) {
		code = patchboard_decode(e->patchboard, code);
		if (debug)
			printf("->patchboard->%c", int_to_char(alphabet, code));
	}

	if (e->rotor_count)
		rotor_tick(e->rotor_array[0]);

	return code;
}

char *enigma_show(Enigma *e)
{
	char *p, *s = malloc(sizeof(char) * (20 + 20 * e->rotor_count));

	p = s;
	p += sprintf(s, "Patchboard->");

	int i;
	for (i = 0; i < e->rotor_count; i++)
		p += sprintf(p, "Rotor(%d,%d)->", e->rotor_array[i]->tickpos,
				e->rotor_array[i]->position);

	p += sprintf(p, "Reflector");

	return s;
}
