/*
 * prettymaze.c
 *
 * The first attempt at writing a real pretty maze
 *
 * written by gattschardo, 24th of February 2009
 */

#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>

#include "dfuncs.h"

/* the maze itself.
 *
 * 0 - no walls
 * 1 - wall above
 * 2 - wall to the left
 * 3 - wall above & to the left
 */
/*const*/ short def_maze[40] = { \
    3, 3, 1, 1, 1, 3, 1, 2, \
    2, 1, 1, 3, 0, 2, 1, 2, \
    3, 2, 1, 0, 1, 1, 2, 2, \
    2, 1, 1, 1, 0, 1, 0, 2, \
    1, 1, 1, 1, 1, 1, 1, 0 };
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
short width = 8; /* this will be totally different in future versions */
int maxsight;		/* will only be changed on terminal resize */
int endsteph, endstepw, c, l;	/* will only be changed on terminal resize */

void init (FILE *mazefile);
void cleanup (void);

int main (int argc, char *argv[])
{
    int input;
    char dm[512];
    FILE *mfile;

    if (argc == 2)
    {
	printf("Initializing from file %s\n",argv[1]);
	if(!(mfile = fopen(argv[1],"r")))
	{
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	init (mfile);
	if(fclose(mfile))
	{
		perror("fclose");
		exit(EXIT_FAILURE);
	}
    }
    else
	init (0);

    for ( draw (); (input = getch ()) != 'x'; draw () )
	switch (input)
	{
	    case 'q': /* turn left */
		dir += 3;
		dir = dir % 4;
		statusline ("You turned left");
		testdraw (0);
		break;
	    case KEY_UP:
	    case 'w': /* move forward */
		if ( mazemove (pos, dir) )
		{
		    statusline ("You moved forward");
		    testdraw (0);
		}
		else
		    statusline ("You couldn't move forward");
		break;
	    case 'e': /* turn right */
		dir++;
		dir = dir % 4;
		statusline ("You turned right");
		testdraw (0);
		break;
	    case KEY_LEFT:
	    case 'a': /* move left */
		if ( mazemove ( pos, (dir+3)%4 ) )
		{
		    statusline ("You moved left");
		    testdraw (0);
		}
		else
		    statusline ("You couldn't move left");
		break;
	    case KEY_DOWN:
	    case 's': /* move backwards */
		if ( mazemove ( pos, (dir+2)%4 ) )
		{
		    statusline ("You moved backwards");
		    testdraw (0);
		}
		else
		    statusline ("You couldn't move back");
		break;
	    case KEY_RIGHT:
	    case 'd': /* move right */
		if ( mazemove ( pos, (dir+1)%4 ) )
		{
		    statusline ("You moved right");
		    testdraw (0);
		}
		else
		    statusline ("You couldn't move right");
		break;
	    case KEY_RESIZE:
		statusline ("Term size changed");
		break;
	    default:
		sprintf (dm, "Unexpected input %i", input);
		statusline (dm);
	}

    exit (EXIT_SUCCESS);
};

void init (FILE *mazefile)
{
    short *ptr;
    char statstr[512];
    struct sigaction on_resz;

    if (mazefile)
    {
	width = 10;
	maze = calloc(100,sizeof(short)); 
	for(ptr=maze;(ptr-maze)<100;ptr++)
		   *ptr = fgetc(mazefile) - 48;
    }
    else
    {
	puts("Using default maze");
	maze = def_maze;
    }

    _pos = maze;
    pos = &_pos;

    on_resz.sa_handler = onresize;
    sigemptyset(&on_resz.sa_mask);
    on_resz.sa_flags = SA_RESTART;
    //on_resz.sa_flags = SA_NODEFER;

    if (sigaction (SIGWINCH, &on_resz, 0) == -1)
    {
	    perror ("sigaction");
	    exit (EXIT_FAILURE);
    }

    atexit (cleanup);

    initscr();
    cbreak();
    noecho();

    getmaxyx (stdscr, l, c);
    findmaxsight (c, l);

    sprintf (statstr,"Init done on %ix%i, maxsight is %i, width %i",
		    l, c, maxsight, width);
    statusline (statstr);
}

void cleanup (void)
{
    if (maze && width!=8)
	free (maze);

    endwin ();
}

/* vim: set foldmethod=syntax: nodigraph: */
