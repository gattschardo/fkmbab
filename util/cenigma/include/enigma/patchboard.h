/* patchboard.h */

#ifndef _PATCHBOARD_H_
#define _PATCHBOARD_H_

#include "util.h"

typedef struct {
	const int *configuration;
} Patchboard;

/* Patchboard *patchboard_create(void); */

Patchboard *patchboard_create(const int *configuration);

void patchboard_delete(Patchboard *p);

int patchboard_encode(const Patchboard *p, int letter);

int patchboard_decode(const Patchboard *p, int letter);

#endif /* !_PATCHBOARD_H_ */
