/* fhashira.c
 *
 * Kyoday's Hashira for the Framebuffer
 *
 * written by gattschardo, April 2009
 */

/* we need a fucking lot of includes: */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

/* local includes */
#include "ftk.h"
#include "blocks.h"
#include "hashlogic.h"

/* GLOBAL vars (yeah I know, they are considered ugly, but wtf */
static int x1, x2, y1, y2;
static block *setblocks[3]; /* nötig wegen drophandler/readhandler sonst würd' ich das auch nicht
			    global haben wollen */
static bool quit, fuckoff;
/* and extern ones (from hashlogic.c) */
extern int field[DEF_W][DEF_H];
extern bool killfield[DEF_W][DEF_H];

/* function prototypes */
int init (int blocksize, int upleft, struct termios *termstate);
void drop_new_set (const int drop);

bool check_touchdown (int xpos, int ypos);
void redraw (void);

void readhandler (int signal);
void drophandler (int signal);

int getxpos (const block *b);
int getypos (const block *b);

/* function definitions, starting with main */
int main (int argc, char **argv)
{
    int i = 0,
	drop_delay = DEFAULT_DELAY,
	match_counter = 0,
	nextlevel = 2;
    struct termios old_termstate;

    if (argc == 1 || argc > 3)
	init (BLOCKSIZE, 0, &old_termstate);
    else if (argc == 2)
	init (atoi (argv[1]), 0, &old_termstate);
    else if (argc == 3)
	init (atoi (argv[1]), atoi (argv[2]), &old_termstate);

    while ( !(quit) && !(check_quit ()) )
    {
	drop_new_set (drop_delay);

	match_counter += calcndrop ();
	if (match_counter >= nextlevel)
	{
	    fprintf (stderr, "%i matches, decreasing delay from %i", match_counter, drop_delay);
	    drop_delay = (98 * drop_delay) / 100;
	    fprintf (stderr, " to %i\n", drop_delay);
	    nextlevel += 2;
	}
    }

    if (gettimer ())
	fprintf (stderr, "Timer still running, not good!\n");

    ftk_quit ();
    tcsetattr (STDIN_FILENO, TCSANOW, &old_termstate);

    printf ("Game finished with %i matches\n", match_counter);

    exit (EXIT_SUCCESS);
}

int init (int blocksize, int upleft, struct termios *termstate)
{
    int x, y;
    struct termios t;
    struct sigaction sa;

    ftk_init ();
    ftk_getmaxxy (&x, &y);

    if ( (blocksize * DEF_H) > y || (blocksize * DEF_W) > x)
    {
	fprintf (stderr, "Blocksize too large for this screen\n");
	exit (EXIT_FAILURE);
    }

    fuckoff = quit = false;
    setblocklen (blocksize);
    setbgcolor (COL_BG);
    x1 = upleft;
    y1 = 0;
    x2 = x1 + DEF_W * blocksize -1;
    y2 = y1 + DEF_H * blocksize -1;
    setgroundline (y2);

    srand48 (time (0));
    memset (field, COL_NONE, sizeof(field));
    memset (killfield, false, sizeof(killfield));

    tcgetattr (STDIN_FILENO, termstate);

    cfmakeraw (&t);
    tcsetattr (STDIN_FILENO, TCSANOW, &t);

    fcntl (STDIN_FILENO, F_SETFL, O_ASYNC|O_NONBLOCK);
    fcntl (STDIN_FILENO, F_SETOWN, getpid ());

    sigemptyset(&sa.sa_mask);
    //sa.sa_flags = SA_RESTART;
    sa.sa_flags = SA_NODEFER;

    sa.sa_handler = readhandler;
    if (sigaction (SIGIO, &sa, 0) == -1)
    {
	    perror ("sigaction");
	    exit (EXIT_FAILURE);
    }

    sa.sa_handler = drophandler;
    if (sigaction (SIGALRM, &sa, 0) == -1)
    {
	    perror ("sigaction");
	    exit (EXIT_FAILURE);
    }

    ftk_drawrect (DRAW_FILLED, x1,y1, x2,y2, COL_BG);

    return 1;
}

void drop_new_set (int delay)
{
    int i, color,
	blocksize = getblocklen ();

    for (i=0; i<3; i++)
    {
	color = lrand48 () % 6;
	
	switch (color)
	{
	    case 0:
		color = C1;
		break;
	    case 1:
		color = C2;
		break;
	    case 2:
		color = C3;
		break;
	    case 3:
		color = C4;
		break;
	    case 4:
		color = C5;
		break;
	    case 5:
		color = C6;
		break;
	}

	setblocks[i] = blockcreate (color, x1 + (DEF_W/2)*blocksize, y1-((i+1)*(blocksize)));
    }

    settimer (delay);

    while (setblocks[0]->falling && !quit)
    {
	pause ();
	fprintf (stderr, "Pause return with quit: %i\n", quit);
    }

    settimer (0);

    for (i=0; i<3; i++)
    {
	field[ getxpos (setblocks[i]) ][ getypos (setblocks[i]) ] = setblocks[i]->color;
	blockremove (setblocks[i]);
	setblocks[i] = 0;
    }
}

bool check_touchdown (int xpos, int ypos)
{
    int blocksize = getblocklen();

    if (ypos+blocksize-1 == y2)
	return true;
    else if (!((ypos-y1) % blocksize) && /* auf einer potentiellen liegeposition */
	    (field[(xpos-x1)/blocksize][((ypos-y1)/blocksize)+1] != COL_NONE))
	return true;
    else
	return false;
}

void redraw (void)
{
    int i,j,
	bs = getblocklen ();

    //ftk_drawrect (DRAW_FILLED, x1,x2, y1,y2, COL_BG);

    for (j=0; j<DEF_H; j++)
	for (i=0; i<DEF_W; i++)
	    if (field[i][j] == COL_NONE)
		ftk_drawrect (DRAW_FILLED, x1+(i*bs),y1+(j*bs), x1+(i*bs)+bs-1,y1+(j*bs)+bs-1, COL_BG);
	    else if (killfield[i][j] == true)
		ftk_drawrect (DRAW_FILLED, x1+(i*bs),y1+(j*bs), x1+(i*bs)+bs-1,y1+(j*bs)+bs-1, COL_GREY);
	    else
		ftk_drawrect (DRAW_FILLED, x1+(i*bs),y1+(j*bs), x1+(i*bs)+bs-1,y1+(j*bs)+bs-1, field[i][j]);

    if (setblocks[0])
    {
	//fputs ("Redrawing falling set\n", stderr);
	for (i=0; i<3; i++)
	    blockredraw (setblocks[i]);
    }
}

void letitfall (int howmany)
{
    int i, j, n,
	c = 0,
	bs = getblocklen ();
    block *array[howmany];

    if (howmany == 0)
    {
	fputs ("You are joking, huh?\n", stderr);
	return;
    }
    //else
	//fprintf (stderr, "Will now drop %i blocks\n", howmany);

    for (j=0; j<DEF_H; j++)
	for (i=0; i<DEF_W; i++)
	    if (killfield[i][j])
	    {
		field[i][j] = COL_NONE;
		for (n=j-1;n>-1 && field[i][n] != COL_NONE;n--)
		    if (!killfield[i][n])
		    {
			fprintf (stderr, "Adding drop candidate %i at %i/%i\n", c, i, n);
			array[c] = blockcreate (field[i][n], x1+i*bs, y1+n*bs);
			field[i][n] = COL_NONE;
			c++;
		    }
	    }

    //fprintf (stderr, "Starting with c = %i\n", c);

    for (j=c; j>0;)
    {
	for (i=0; i<c; i++)
	    if (array[i]->falling)
		if (check_touchdown (array[i]->xpos, array[i]->ypos))
		{
		    fprintf (stderr, "Touchdown %i at (%i/%i)\n", i, array[i]->xpos, array[i]->ypos);
		    array[i]->falling = false;
		    field[ getxpos(array[i]) ][ getypos(array[i]) ] = array[i]->color;
		    j--;
		}
		else
		{
		    //fprintf (stderr, "%i still falling at (%i/%i)\n", i, array[i]->xpos, array[i]->ypos);
		    blockfall (array[i]);
		    if (!(array[i]->falling))
		    {
			fprintf (stderr, "Touchdown %i at (%i/%i)\n", i, array[i]->xpos, array[i]->ypos);
			field[ getxpos(array[i]) ][ getypos(array[i]) ] = array[i]->color;
			j--;
		    }
		}

	usleep (5000);
    }

    for (i=0; i<c; i++)
	blockremove (array[i]);
}

void readhandler (int signal)
{
    static int delay = 0; /* for pausing */

    int i = 0, /* this needs to be done for read to work correctly */
	blocksize = getblocklen ();

    if ( (read (STDIN_FILENO, &i, 1) == -1) || fuckoff)
	return;
    else
	fuckoff = true;

    switch (i)
    {
	case 'a': /* left */
	    if (setblocks[0] && setblocks[0]->xpos > x1 &&
		    field[ getxpos(setblocks[0]) -1][ getypos(setblocks[0]) +1] == COL_NONE)
		for (i=0; i<3; i++)
		{
		    blockclear (setblocks[i]);
		    setblocks[i]->xpos -= blocksize;
		    blockredraw (setblocks[i]);
		}
	    break;
	case 'd': /* right */
	    if (setblocks[0] && setblocks[0]->xpos+blocksize < x2 &&
		    field[ getxpos(setblocks[0]) +1][ getypos(setblocks[0]) +1] == COL_NONE)
		for (i=0; i<3; i++)
		{
		    blockclear (setblocks[i]);
		    setblocks[i]->xpos += blocksize;
		    blockredraw (setblocks[i]);
		}
	    break;
	case 'w': /* colors downw */
	    if (setblocks[0])
	    {
		i = setblocks[0]->color;
		setblocks[0]->color = setblocks[1]->color;
		setblocks[1]->color = setblocks[2]->color;
		setblocks[2]->color = i;
		for (i=0; i<3; i++)
		    blockredraw (setblocks[i]);
	    }
	    break;
	case 'e': /* colors upw */
	    if (setblocks[0])
	    {
		i = setblocks[2]->color;
		setblocks[2]->color = setblocks[1]->color;
		setblocks[1]->color = setblocks[0]->color;
		setblocks[0]->color = i;
		for (i=0; i<3; i++)
		    blockredraw (setblocks[i]);
	    }
	    break;
	case 's': /* drop */
	    fuckoff = false;
	    for (i=0; i<blocksize; i++)
		raise (SIGALRM);
	    break;
	case 'r':
	    redraw ();
	    break;
	case 'p':
	    if (delay)
	    {
		fprintf (stderr, "Continuing game\n");
		settimer (delay);
		delay = 0;
	    }
	    else
	    {
		fprintf (stderr, "Pausing game\n");
		delay = gettimer ();
		settimer (0);
	    }
	    break;
	case 'q': /* quit */
	    fputs ("Quitting game\n", stderr);
	    quit = true;
	    //settimer (0);
	    //setblocks[0]->falling = false;
	    break;
	default:
	    fprintf (stderr, "Unknown input %c\n", i);
    }

    fuckoff = false;
}

void drophandler (int signal)
{
    int i;

    if (fuckoff || quit || !setblocks[0] || !setblocks[0]->falling)
	return;
    else
	fuckoff = true;

    if (check_touchdown (setblocks[0]->xpos, setblocks[0]->ypos))
	setblocks[0]->falling = false; /* let drop_new_set know */
    else
	for (i=0; i<3; i++)
	    blockfall (setblocks[i]);

    fuckoff = false;
}

void killblocks (int color)
{
    int i,j,
	bs = getblocklen ();

    for (j=0; j<DEF_H; j++)
	for (i=0; i<DEF_W; i++)
	    if (killfield[i][j])
		ftk_drawrect (DRAW_FILLED, x1+(i*bs),y1+(j*bs), x1+(i*bs)+bs-1,y1+(j*bs)+bs-1, color);
}

int getxpos (const block *b)
{
    int bs = getblocklen ();

    return ((b->xpos -x1)/bs);
}

int getypos (const block *b)
{
    int y,
	bs = getblocklen ();

    if (b->ypos-y1 == 0)
	return 0;
    else if (b->ypos-y1 % bs)
	y = (b->ypos -y1)/bs;
    else
	y =((b->ypos -y1)/bs)-1;

    if (y < 0 || y > DEF_H)
	return 0;

    return y;
}

/* that's all */
