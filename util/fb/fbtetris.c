/* fbtetris.c
 *
 * Tetris for the framebuffer
 *
 * written by gattschardo, April 2009
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ftk.h"
#include "blocks.h"

#define BLOCKSIZE 10

int main (void)
{
    int counter;

    ftk_init ();

    setblocklen (BLOCKSIZE);
    setbgcolor (COL_BLACK);
    setgroundline (299);
    counter = 0;

    block *last, *green, *testy = blockcreate (COL_RED, 0, 0);
    last = blockcreate (COL_YELLOW, 2*BLOCKSIZE, 0);
    
    while (last->falling)
    {
	counter++;
	usleep (125000);
	blockfall (testy);
	if (counter == 3)
	    green = blockcreate (COL_GREEN, BLOCKSIZE, 0);

	if (counter > 3)
	    blockfall (green);
	if (counter > 27)
	    blockfall (last);
    }

    exit (EXIT_SUCCESS);
}
