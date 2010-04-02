/* enigma.h */

#ifndef _ENIGMA_H_
#define _ENIGMA_H_

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "patchboard.h"
#include "rotor.h"
#include "reflector.h"

#define DEFRCFILE (".enigmarc")

typedef struct {
	Patchboard *patchboard;
	Rotor **rotor_array;
	Reflector *reflector;
	int rotor_count;
} Enigma;

extern bool debug;

Enigma *enigma_create(int *patchboardconf, int *reflectorconf, int rotor_count,
		int **rotor_permutations, int *rotor_ticks, int *rotor_stars);

void enigma_delete(Enigma *e);

int enigma_encode(Enigma *e, int letter);

char *enigma_show(Enigma *e);

#endif /* !_ENIGMA_H_ */
