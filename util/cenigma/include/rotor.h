/* rotor.h */

#ifndef _ROTOR_H_
#define _ROTOR_H_

#include "util.h"

typedef struct Rotor {
	const int *configuration;
	int tickpos;
	int conflen;
	int position;
	struct Rotor *next;
} Rotor;

Rotor *rotor_create(const int *configuration, int tickpos, int position,
		Rotor *next);

void rotor_delete(Rotor *r);

int rotor_encode(const Rotor *r, int letter);

int rotor_decode(const Rotor *r, int letter);

void rotor_tick(Rotor *r);

#endif /* !_ROTOR_H_ */
