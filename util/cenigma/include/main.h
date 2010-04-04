/* main.h */

#ifndef _ENIGMA_MAIN_H_
#define _ENIGMA_MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>

#include "enigma.h"
#include "util.h"

#define DEFRCFILE (".enigmarc")
#define BUFSIZE (512)

Enigma *buildfromfile(const char *file);

char *eat_space(char *p);

void usage(const char *progname);

#endif /* !_ENIGMA_MAIN_H_ */
