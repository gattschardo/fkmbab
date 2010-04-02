/* reflector.h */

#ifndef _REFLECTOR_H_
#define _REFLECTOR_H_

#include "util.h"

typedef struct Reflector {
	const int *configuration;
} Reflector;

Reflector *reflector_create(const int *configuration);

void reflector_delete(Reflector *r);

int reflector_encode(const Reflector *r, int letter);

/* int reflector_decode(const Reflector *r, int letter); */

#endif /* !_REFLECTOR_H_ */
