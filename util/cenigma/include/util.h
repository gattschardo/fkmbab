/* util.h
 *
 * Utility functions for enigma
 */
#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* global defines */

#define DEFALPHABET ("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
#define DEFPATCHBOARD ("JWULCMNOHPQZYXIRADKEGVBTSF")
#define DEFREFLECTOR ("IMETCGFRAYSQBZXWLHKDVUPOJN")

extern bool debug;
const char *alphabet;

int char_to_int(const char *alphabet, char letter);

char int_to_char(const char *alphabet, int letter);

int *string_to_int(const char *alphabet, const char *word);

bool word_in_alphabeth(const char *alphabet, const char *word);

bool char_in_alphabeth(const char *alphabet, char c);

#endif /* !_UTIL_H_ */
