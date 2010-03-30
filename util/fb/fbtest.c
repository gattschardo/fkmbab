/* fbtest.c
 *
 * Testing program
 *
 * written by gattschardo, April 2009
 */

#include <stdio.h>
#include <stdlib.h>

#include "ftk.h"

int main (void)
{
	int x,y;

	ftk_init ();

	ftk_getmaxxy (&x, &y);
	printf ("Got a %ix%i screen\n", x, y);

	x--;
	y--;

	//ftk_drawrawpix (0, 0, 100, 0, 150);
	//ftk_drawrawpix (10, 10, 100, 255, 150);

	ftk_drawhline (0, x, y/2, COL_WHITE);
	ftk_drawvline (x/2, 0, y, COL_WHITE);

	ftk_drawrect (DRAW_SKEL, 0,0, x,y, COL_WHITE);

	ftk_drawrect (DRAW_FILLED, 1,1, (x/2)-1,(y/2)-1, COL_RED);
	ftk_drawrect (DRAW_FILLED, (x/2)+1,1, x-1,(y/2)-1, COL_BLUE);
	ftk_drawrect (DRAW_FILLED, 1,(y/2)+1, (x/2)-1,(y/1)-1, COL_GREEN);
	ftk_drawrect (DRAW_FILLED, (x/2)+1,(y/2)+1, x-1,(y/1)-1, COL_YELLOW);

	ftk_quit ();

	exit(EXIT_SUCCESS);
}
