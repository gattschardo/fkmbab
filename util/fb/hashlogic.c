/* hashlogic.c */

#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <string.h>

#include "hashlogic.h"
#include "ftk.h" /* ONLY for colors */

/* Global Vars */
int field[DEF_W][DEF_H];
bool killfield[DEF_W][DEF_H];

int drop (void)
{
    int i, j, n,
	total = 0,
	matches = 0,
	bs = getblocklen ();
    
    fputs ("Drop started\n", stderr);

    for (j=0; j<DEF_H; j++)
	for (i=0; i<DEF_W; i++)
	    if (killfield[i][j])
	    {
		matches++;
		for (n=j-1;n>-1 && field[i][n] != COL_NONE;n--)
		    if (killfield[i][n])
			break;
		    else
			total++;
	    }

    if (total)
	letitfall (total);
    else
	for (j=0; j<DEF_H; j++)
	    for (i=0; i<DEF_W; i++)
		if (killfield[i][j])
		    field[i][j] = COL_NONE;

    fprintf (stderr, "%i matches\n", matches);
    return matches;
}

int calcndrop (void)
{
    int col, lin,
	drops = 1,
	dtotal = 0;

    while (drops)
    {
	drops = 0;

	for (lin = 0; lin < DEF_H; lin++)
	    for (col = 0; col < DEF_W; col++)
		if (field[col][lin] != COL_NONE)
		{
		    drops += check_line (col, lin);
		    drops += check_col (col, lin);
		    drops += check_diagdown (col, lin);
		    drops += check_diagup (col, lin);
		}

	if (drops)
	{
	    fprintf (stderr, "Found %i places with matches\n", drops);
	    killblocks (COL_BG);
	    //sleep (1);
	    dtotal += drop ();
	}

	memset (killfield, false, sizeof(killfield));
    }

    return dtotal;
}

bool check_quit ()
{
    int i;

    for (i=0; i<DEF_W; i++)
	if (field[i][0] != COL_NONE)
	    return true;

    return false;
}

int check_line (int x, int y)
{
    int color, counter;

    if ((color = field[x][y]) == COL_NONE)
    {
	fprintf (stderr, "Invalid field %i/%i\n", x, y);
	return 0;
    }

    for (counter=0; x+counter<DEF_W && (field[x+counter][y] == color); counter++);

    if (counter < 3)
	return 0;
    else
	for (color=0; color<counter; color++)
	    killfield[x+color][y] = true;

    return 1;
}

int check_col (int x, int y)
{
    int color, counter;

    if ((color = field[x][y]) == COL_NONE)
    {
	fprintf (stderr, "Invalid field %i/%i\n", x, y);
	return 0;
    }

    for (counter=0; y+counter<DEF_H && field[x][y+counter] == color; counter++);

    if (counter < 3)
	return 0;
    else
	for (color=0; color<counter; color++)
	    killfield[x][y+color] = true;

    return 1;
}

int check_diagdown (int x, int y)
{
    int color, counter;

    if ((color = field[x][y]) == COL_NONE)
    {
	fprintf (stderr, "Invalid field %i/%i\n", x, y);
	return 0;
    }

    for (counter=0; x+counter<DEF_W && y+counter<DEF_H && field[x+counter][y+counter] == color; counter++);

    if (counter < 3)
	return 0;
    else
	for (color=0; color<counter; color++)
	    killfield[x+color][y+color] = true;

    return 1;
}

int check_diagup (int x, int y)
{
    int color, counter;

    if ((color = field[x][y]) == COL_NONE)
    {
	fprintf (stderr, "Invalid field %i/%i\n", x, y);
	return 0;
    }

    for (counter=0; x+counter<DEF_W && y-counter>-1 && field[x+counter][y-counter] == color; counter++);

    if (counter < 3)
	return 0;
    else
	for (color=0; color<counter; color++)
	    killfield[x+color][y-color] = true;

    return 1;
}

void settimer (unsigned int delay)
{
    struct itimerval timer;

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = delay;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = delay;
    
    setitimer (ITIMER_REAL, &timer, 0);
}

int gettimer (void)
{
    struct itimerval timer;

    getitimer (ITIMER_REAL, &timer);

    return timer.it_interval.tv_usec;
}
