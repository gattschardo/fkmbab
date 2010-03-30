/* dfuncs.c
 *
 * drawing functions
 */

#include <stdbool.h>
#include <stdlib.h>

#ifdef STDIO
# include <stdio.h>
/* can only have one */
# undef NCURSES
#endif /* STDIO */

#ifdef NCURSES
# include <ncurses.h>
# include <string.h> /* for memset */
#endif /* NCURSES */

#include "dfuncs.h"

bool mazemove (short **lpos, int direction)
{
    /* NOTE: We always expect the maze to be completely surrounded by walls.
     * If it isn't, it can crash in this function!
     */
    switch (direction)
    {
	case 0: /* up */
	    if ( **lpos != 1 && **lpos != 3)
		(*lpos) -= width;
	    else
		return false;
	    break;
	case 1: /* right */
	    if ( ((short)(*lpos-maze)+1 % width) && *(*lpos+1) != 2 && *(*lpos+1) != 3)
		(*lpos)++;
	    else
		return false;
	    break;
	case 2: /* down */
	    if ( *(*lpos+width) != 1 && *(*lpos+width) != 3)
		(*lpos) += width;
	    else
		return false;
	    break;
	case 3: /* left */
	    if ( ((short)(*lpos-maze) % width) && **lpos != 2 && **lpos != 3)
		(*lpos)--;
	    else
		return false;
	    break;
	default:
	    return false;
    }

    return true;
}

bool wall (int step, int position)
{
    bool result;
    int i;
    short *_lpos = *pos,
	**lpos = &_lpos;

    for (i=0; i<step; i++)
	mazemove (lpos, dir);

    if (position) /* right */
	result = !(mazemove (lpos, (dir+1)%4 ));
    else /* left */
	result = !(mazemove (lpos, (dir+3)%4 ));

    return result;
}

int findmaxsight (int cols, int rows)
{
    if (cols < 11 || rows < 10)
	maxsight = 0;

    rows -= 3;

    if ( (cols / 8) < (rows / 7) )
	maxsight = 0;

    maxsight = rows / 7;
    endsteph = rows+2 % 7;
    endstepw = cols % 8;
   
    return maxsight;
}

int findstopwall (int step, int position)
{
    int i;
    short *_lpos = *pos,
	**lpos = &_lpos;

    for (i=0; i<step; i++)
	mazemove(lpos, dir);

    if (position == 0) /* left */
	mazemove (lpos, (dir+3)%4 );
    else if (position == 2) /* right */
	mazemove (lpos, (dir+1)%4 );

    for (i=0; mazemove (lpos, dir); i++);

    return i;
}

void drawwall (bool mode, int step, int position)
{
#ifdef NCURSES
    int sl = step * 4;
#endif /* NCURSES */

    if (position)
#ifdef STDIO
    {
	if (mode)
	    printf ("Draw wall on the right side at step %i\n", step);
	else
	    printf ("Clear wall on the right side at step %i\n", step);
    }
#endif /* STDIO */
#ifdef NCURSES
    {
	if (mode)
	{
	    mvaddch (sl, c-sl-1, '/');
	    mvaddch (sl+1, c-sl-2, '/');
	    mvaddch (sl+2, c-sl-3, '/');

	    mvaddch (l-sl-2, c-sl-1, '\\');
	    mvaddstr (l-sl-3, c-sl-2, "\\ ");
	    mvaddstr (l-sl-4, c-sl-3, "\\  ");
	}
	else
	{
	    mvaddch (sl, c-sl-1, ' ');
	    mvaddch (sl+1, c-sl-2, ' ');
	    mvaddch (sl+2, c-sl-3, ' ');

	    mvaddch (l-sl-2, c-sl-1, ' ');
	    mvaddch (l-sl-3, c-sl-2, ' ');
	    mvaddch (l-sl-4, c-sl-3, ' ');
	}
    }
#endif /* NCURSES */
    else
#ifdef STDIO
    {
	if (mode)
	    printf ("Draw wall on the left side at step %i\n", step);
	else
	    printf ("Clear wall on the left side at step %i\n", step);
    }
#endif /* STDIO */
#ifdef NCURSES
    {
	if (mode)
	{
	    mvaddch (sl, sl, '\\');
	    mvaddch (sl+1, sl+1, '\\');
	    mvaddch (sl+2, sl+2, '\\');

	    mvaddch (l-sl-2, sl, '/');
	    mvaddstr (l-sl-3, sl, " /");
	    mvaddstr (l-sl-4, sl, "  /");
	}
	else
	{
	    mvaddch (sl, sl, ' ');
	    mvaddch (sl+1, sl+1, ' ');
	    mvaddch (sl+2, sl+2, ' ');

	    mvaddch (l-sl-2, sl, ' ');
	    mvaddch (l-sl-3, sl+1, ' ');
	    mvaddch (l-sl-4, sl+2, ' ');
	}
    }
#endif /* NCURSES */
}

void drawstopwall (bool mode, int step, int distance, int position)
{
#ifdef NCURSES
    int i, j, sl = step * 4;

    distance *= 4;
#endif /* NCURSES */

    switch (position)
    {
	case 0:
#ifdef STDIO
	    if (mode)
		printf ("Draw stopwall on the left side at step %i at distance %i\n", step, distance);
	    else
		printf ("Clear stopwall on the left side at step %i at distance %i\n", step, distance);
#endif /* STDIO */
#ifdef NCURSES
	    if (mode)
	    {
		mvaddstr (distance+sl+2, sl, "___");
		mvaddstr (l-distance-sl-5, sl, "___");

		for(i=l-distance-sl-4; i<l-sl-4; i++)
		    mvaddstr (i, sl, "###");
	    }
	    else
	    {
		mvaddstr (distance+sl+2, sl, "   ");
		mvaddstr (l-distance-sl-5, sl, "   ");
	    }
#endif /* NCURSES */
	    break;
	case 1:
#ifdef STDIO
	    if (mode)
		printf ("Draw stopwall in the center at distance %i\n", distance);
	    else
		printf ("Clear stopwall in the center at distance %i\n", distance);
#endif /* STDIO */
#ifdef NCURSES
	    for(i=4+sl;i<=c-sl-5;i++)
	    {
		if (mode)
		{
		    mvaddch (sl+2, i, '_');
		    for (j=sl+3; j<l-sl-5; j++)
			mvaddch(j, i, ' ');
		    mvaddch (l-sl-5, i, '_');
		}
		else
		{
		    mvaddch (sl+2, i, ' ');
		    mvaddch (l-sl-5, i, ' ');
		}
	    }
#endif /* NCURSES */
	    break;
	case 2:
#ifdef STDIO
	    if (mode)
		printf ("Draw stopwall on the right side at step %i at distance %i\n", step, distance);
	    else
		printf ("Clear stopwall on the right side at step %i at distance %i\n", step, distance);
#endif /* STDIO */
#ifdef NCURSES
	    if (mode)
	    {
		mvaddstr (distance+sl+2, c-sl-3, "___");
		mvaddstr (l-distance-sl-5, c-sl-3, "___");

		for(i=l-distance-sl-4; i<l-sl-4; i++)
		    mvaddstr (i, c-sl-3, "###");
	    }
	    else
	    {
		mvaddstr (distance+sl+2, c-sl-3, "   ");
		mvaddstr (l-distance-sl-5, c-sl-3, "   ");
	    }
#endif /* NCURSES */
	    break;
	default:
#ifdef STDIO
	    puts ("Invalid position for stopwall");
#endif /* STDIO */
#ifdef NCURSES
	    statusline ("Invalid position for stopwall");
#endif /* NCURSES */
    }
}

void drawposts (bool mode, int step)
{
#ifdef NCURSES
    int i, sl = step * 4;
#endif /* NCURSES */

#ifdef STDIO
    if (mode)
	printf ("Draw posts at step %i\n", step);
    else
	printf ("Clear posts at step %i\n", step);
#endif /* STDIO */
#ifdef NCURSES
    for (i=sl+3; i<l-sl-4; i++)
    {
	if (mode)
	{
	    mvaddstr (i, sl, "   |");
	    mvaddstr (i, c-sl-4, "|   ");
	}
	else
	{
	    mvaddch (i, sl+3, ' ');
	    mvaddch (i, c-sl-4, ' ');
	}
    }
#endif /* NCURSES */
}

void draw (void)
{
    int i,j, stopwall, sight;
#ifdef NCURSES
    int k;
#endif

    drawfloor (1);

    sight = findstopwall (0, 1);
    if (sight > maxsight-1)
	sight = maxsight-1;
    else
	drawstopwall (1, sight, sight, 1);

    for (i=0; i<sight+1; i++)
    {
	drawposts (1, i);

	for (j=0; j<3; j+=2)
	{
	    if ( wall (i, j) )
		drawwall (1, i, j);
	    else if ( i+(stopwall = findstopwall(i, j) ) <= maxsight-1 )
		drawstopwall (1, i, stopwall, j);
#ifdef NCURSES
	    else
		for(k=l/2; k<l-(i*4)-4; k++)
		{
		    if (j)
			mvaddstr (k, c-(i*4)-3, "###");
		    else
			mvaddstr (k, (i*4), "###");
		}
#endif
	}
    }
}

void testdraw (bool mode)
{
    int i,j;

    drawfloor (mode);

    for (i=0;i<maxsight;i++)
    {
	drawposts (mode, i);

	drawstopwall (mode, i, i, 1);

	for (j=0;j<(maxsight-i);j++)
	{
	    drawstopwall(mode, i, j, 0);
	    drawstopwall(mode, i, j, 2);
	}

	drawwall (mode, i, 0);
	drawwall (mode, i, 2);

#ifdef NCURSES
	if (mode)
	{
	    statusline ("Finished step");
	    getch();
	}
#endif /* NCURSES */
    }

#ifdef NCURSES
    if (mode)
	statusline ("Statusline test ... OK!");

    refresh();
#endif /* NCURSES */
}

void drawfloor (bool mode)
{
#ifdef STDIO
    puts ("Draw floor");
#endif
#ifdef NCURSES
    int i,j;

    for (i = l/2; i<l-1; i++)
	for (j=0; j<c; j++)
	{
	    if (mode)
		mvaddch (i, j, '#');
	    else
		mvaddch (i, j, ' ');
	}
#endif
}

#ifdef NCURSES
void statusline (const char *string)
{
    char eraser[c+1];

    memset (eraser, ' ', c);
    eraser[c] = '\0';
    mvaddstr (l-1, 0, eraser);

    mvaddstr (l-1, 0, string);
}
#endif /* NCURSES */

#ifdef NCURSES
void onresize (int signal)
{
    static int counter = 0;
    char msg[512];

    endwin ();
    initscr ();
    cbreak ();
    noecho ();
    clear ();

    getmaxyx (stdscr, l, c);
    findmaxsight (c, l);

    sprintf (msg, "%i: Handled resize signal %i, new geo is %ix%i",
		    counter++, signal, l, c);
    statusline (msg);
    //drawposts (1,1);
    testdraw (0);

    //fputs ("muH",stderr);
}
#endif /* NCURSES */

/* vim: set nodigraph foldmethod=syntax: */
