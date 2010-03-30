/* nct.c
 *
 * ncurses test program
 */

#include <ncurses.h>
#include <stdlib.h>

#include "dfuncs.h"

/* the maze itself.
 *
 * 0 - no walls
 * 1 - wall above
 * 2 - wall to the left
 * 3 - wall above & to the left
 */
/*short maze[40] = { \
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
short width = 8; /* this will be totally different in future versions */
int endsteph, endstepw, c, l;	/* will only be changed on terminal resize */

void init (void);
void cleanup (void);

int main()
{
    init ();

    while (getch () != 'x')
    {
	testdraw (1);
	getch();
	testdraw (0);
	statusline ("Press x to exit now!");
    }

    cleanup ();

    exit (EXIT_SUCCESS);
}

void init (void)
{
    initscr();
    cbreak();
    noecho();

    getmaxyx (stdscr, l, c);
    findmaxsight (c, l);
}

void cleanup (void)
{
    endwin ();
}

/* vim: set nodigraph foldmethod=syntax: */
