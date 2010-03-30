/*
 * pmt.c
 *
 * pretty maze test
 *
 * written 22nd of February 2009 by gattschardo
 */

#include <stdio.h>  /* will not be needed for an ncurses version. */
#include <stdlib.h> /* only needed for exit() and EXIT_SUCCESS */

#include "dfuncs.h"

/* GLOBAL variables follow. pretty maze uses quite a lot of them, and not all
 * of them are const, so be careful if you want to mess around in here! */

/* these two will not be needed in a perfect version */
#define WIDTH 10
#define HEIGHT 10

/* the maze itself.
 *
 * 0 - no walls
 * 1 - wall above
 * 2 - wall to the left
 * 3 - wall above & to the left
 */
/*const int maze[40] = { \
    3, 3, 1, 1, 1, 3, 1, 2, \
    2, 1, 1, 3, 0, 2, 1, 2, \
    3, 2, 1, 0, 1, 1, 2, 2, \
    2, 1, 1, 1, 0, 1, 0, 2, \
    1, 1, 1, 1, 1, 1, 1, 0 };*/
short *maze;

/* player position in the maze.
 * points to the field (s)he stands on
 */
short *_pos; /* upper left corner */
short **pos; /* these two are a bit tricky. TODO explain more */

/* direction into which the player looks.
 * 
 * 0 - up
 * 1 - right
 * 2 - down
 * 3 - left
 */
int dir = 2; /* looks down */

/* maximum sight & others */
int maxsight;		/* will only be changed on terminal resize */
short width = WIDTH; /* this will be totally different in future versions */
int endsteph, endstepw;	/* will only be changed on terminal resize */

/* prototypes */
/* the ncurses version will use this to init ncurses, too */
void init (const char *filename);
/* the ncurses version will not need this */
void dump (void);
//bool mazemove (short **lpos, int direction);

/* main function definition. will be different with ncurses */
int main (int argc, char *argv[])
{
    int input;
    short *backup;

    if (argc != 2)
    {
	    printf ("Usage: %s filename\n", argv[0]);
	    exit (EXIT_FAILURE);
    }

    init (argv[1]);
    dump ();
    
    backup = maze;
    while ( (input = getchar()) != 'x' )
    {
	switch (input)
	{
	    case '\n': /* do nothing (and don't paint) */
		continue;
	    case 'q': /* turn left */
		dir += 3;
		dir = dir % 4;
		break;
	    case 'w': /* move forward */
		mazemove (pos, dir);
		break;
	    case 'e': /* turn right */
		dir++;
		dir = dir % 4;
		break;
	    case 'a': /* move left */
		mazemove ( pos, (dir+3)%4 );
		break;
	    case 's': /* move backwards */
		mazemove ( pos, (dir+2)%4 );
		break;
	    case 'd': /* move right */
		mazemove ( pos, (dir+1)%4 );
		break;
	    default:
		puts ("Invalid input");
		continue;
	}

	dump();
	draw();
	printf ("Dir: %i Go: ", dir);
    }

    if (backup == maze)
	free (maze);
    else
	puts ("We fucked up our maze pointer!");

    puts("END");

    exit (EXIT_SUCCESS);
}

/* other funtion definitions */
void init (const char *filename)
{
    short *ptr;
    FILE *fhdl;

    printf ("init from file %s... ", filename);

    if ( !(fhdl = fopen (filename, "r") ) )
    {
	    perror ("fopen");
	    exit (EXIT_FAILURE);
    }

    maze = calloc ( WIDTH*HEIGHT, sizeof(short) );
    _pos = maze;
    pos = &_pos;

    for (ptr = maze; (ptr - maze) < WIDTH*HEIGHT; ptr++)
    {
	    *ptr = fgetc (fhdl) - 48;
	    putchar (*ptr);
    }

    if ( fclose (fhdl) )
    {
	    perror ("fclose");
	    exit (EXIT_FAILURE);
    }

    puts ("done. Use 'x' to exit.");
}

void dump (void)
{
    int i,j;
    short *ptr = maze;

    for (i=0; i<(2*HEIGHT); i++)
    {
	for (j=0; j<WIDTH; j++)
	{
	    if ( !(i % 2) ) /* Vollreihe */
	    {
		putchar ('+');
		if ( *ptr == 1 || *ptr == 3 )
		    putchar ('-');
		else putchar (' ');

		ptr++;
	    }
	    else /* Leerreihe */
	    {
		if ( *ptr == 2 || *ptr == 3 )
		    putchar ('|');
		else putchar (' ');

		if ( ptr != *pos )
		    putchar (' ');
		else switch (dir)
		{
		    case 0:
			putchar ('^');
			break;
		    case 1:
			putchar ('>');
			break;
		    case 2:
			putchar ('v');
			break;
		    case 3:
			putchar ('<');
			break;
		    default:
			putchar ('x');
		}

		ptr++;
	    }
	}

	if ( !(i % 2) )
	    ptr -= WIDTH;

	putchar ('\n');
    }
}

/* vim: set nodigraph foldmethod=syntax: */
