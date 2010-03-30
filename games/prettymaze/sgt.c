/*
 * sgt.c
 *
 * seedgeneratortest.c
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define W 10
#define H 10

char *seed;

unsigned long long genseed (void);
short *genmaze (short *maze);
short getfromseed (void);

void dump (short *maze, short *stat);

int main (int argc, char *argv[])
{
    //int i;
    bool gotseed = false;
    short *maze = malloc (sizeof(short)*H*W);

    if (argc == 2)
    {
	seed = argv[1];
	gotseed = true;
    }
    else
	genseed ();
    
    printf ("Seed is %s\n", seed);

    /*for (i=0;i<160;i++)
	printf ( "%i", getfromseed () );
    puts ("");*/
    genmaze (maze);
    dump (maze, 0);

    if (!gotseed)
	free (seed);

    free (maze);
    exit (EXIT_SUCCESS);
}

short *genmaze (short *maze)
{
    bool accepted;
    short *m_ptr = maze,
	  *states = malloc (sizeof(short)*H*W),
	  *s_ptr = states,
	  waynum, i, lb=0;

    memset (states, sizeof(*states), 0);

    *m_ptr = 3; /* upper left corner */
    *s_ptr = waynum = 1; /* way number 1 */

    for (m_ptr++, s_ptr++; (m_ptr-maze) < (H*W -W); m_ptr++, s_ptr++)
    {
	if ( !((m_ptr-maze+1) % W) ) /* right border */
	{
	    *(s_ptr-1) = -*(s_ptr-1);
	    *m_ptr = 2;
	}
	else
	{
	    accepted = false; /* prepare for looping */
	    while(!accepted)
	    {
		*m_ptr = getfromseed ();

		if ( (m_ptr-maze) < W ) /* at upper border */
		{
		   if (*m_ptr == 0 || *m_ptr == 2) /* upper border open */
		    continue;
		   else
		   {
		       if (*m_ptr == 3)
		       {
			   /* this starts a new way */
			   *(s_ptr-1) = -*(s_ptr-1);
			   *s_ptr = ++waynum;
		       }
		       else /* take waynumber from field before */
			   *s_ptr = *(s_ptr-1);

		       accepted = true;
		       continue;
		   }
		}

		if ( !((m_ptr-maze) % W) ) /* at left border */
		{
		   if (*m_ptr == 0 || *m_ptr == 1) /* left border open */
		       continue;
		   else
		   {
		       if ( *(s_ptr-W) < 0 && *m_ptr == 3 )
			   continue; /* cannot cut off an area that needs connection */
		       else if ( *m_ptr == 3 ) /* this starts a new way */
			   *s_ptr = ++waynum;
		       else if (*(s_ptr-W)<0) /* take waynumber from field above, but make sure it's positive */
			   *s_ptr = -*(s_ptr-W);
		       else /* it is already positive */
			   *s_ptr = *(s_ptr-W);

		       accepted = true;
		       //lb++;
		       continue;
		   }
		}

		/* in the middle of the maze */

		if ( *(s_ptr-W) < 0 ) /* field above needs connection */
		    if ( *(s_ptr-1) == -*(s_ptr-W) ) /* same waynumbers to the left and above */
		    {
			if (*m_ptr == 0)
			    continue; /* must not create holes */
			else
			{
			    if (*m_ptr==3)
			    {
				*(s_ptr-1) = -*(s_ptr-1);
				*s_ptr = ++waynum; /* new way */
			    }
			    else /* take waynumber from the left (because above us it's negative) */
				*s_ptr = *(s_ptr-1);

			    accepted = true;
			}
		    }
		    else if (*m_ptr == 1 || *m_ptr == 3) /* cannot cut off an area */
			continue;
		    else if (*m_ptr == 0) /* field above needs connection and got it */
		    {
			if ( *(m_ptr-W) > 1 || *(m_ptr-1) == 1 || *(m_ptr-1) == 3)
			{
			/* decide which waynumber our field gets (the lower one) */
			    if ( *(s_ptr-1) < -*(s_ptr-W) )
				*s_ptr = *(s_ptr-1);
			    else
				*s_ptr = -*(s_ptr-W);

			    accepted = true;
			}
			else
			    continue;
		    }
		    else /* field = 2 */
		    {
			*(s_ptr-1) = -*(s_ptr-1);
			*s_ptr = -*(s_ptr-W); /* take waynumber from above, but make it positive */

			accepted = true;
		    }
		else /* field above doesn't need connection */
		{
		    if ( *(s_ptr-1) == *(s_ptr-W) ) /* same waynumber */
		    {
			if (*m_ptr==0) /* must not create holes */
			    continue;
			else
			{
			    if (*m_ptr==3)
			    {
				/* *(s_ptr-1) = -*(s_ptr-1); // not sure if we need this */
				*s_ptr = ++waynum;
			    }
			    else
				*s_ptr = *(s_ptr-1);

			    accepted = true;
			}
		    }
		    else
		    {
			if (*m_ptr == 3) /* new waynumber */
			{
			    *(s_ptr-1) = -*(s_ptr-1);
			    *s_ptr = ++waynum;
			}
			else if (*m_ptr == 2) /* take waynumber from above */
			{
			    *(s_ptr-1) = -*(s_ptr-1);
			    *s_ptr = *(s_ptr-W);
			}
			else if (*m_ptr == 0)
			{
			    if ( *(m_ptr-W) > 1 || *(m_ptr-1) == 1 || *(m_ptr-1) == 3)
			    {
				if (*(s_ptr-1) < *(s_ptr-W)) /* take lower waynumber */
				    *s_ptr = *(s_ptr-1);
				else
				    *s_ptr = *(s_ptr-W);
			    }
			    else
				continue;
			}
			else
			    *s_ptr = *(s_ptr-1);

			accepted = true;
		    }
		}
	    }
	}
    }

    /* cleanup end row: */
    for (m_ptr -= W-1, s_ptr -= W-2; (m_ptr-maze+1) < (H*W-W); m_ptr++, s_ptr++)
    {
	if (*(s_ptr-1) < 0) /* need to make connection */
	{
		//*m_ptr = 3; /* yep it works */
		lb++;
	}
    }

    for(i=1, m_ptr++; i<W; i++, m_ptr++)
	*m_ptr = 1;

    *(++m_ptr) = 0;

    //dump (maze, states);

    printf ("Had %i endrow-cases\n", lb);

    free (states);

    return maze;
}

short getfromseed (void)
{
    static int lastpos = -1;
    char *pos = seed;

    lastpos++;
    pos += lastpos;

    if (!*pos)
    {
	pos = seed;
	lastpos = 0;
    }

    switch (*pos)
    {
	case '1':
	    return 0;
	case '2':
	    return 1;
	case '3':
	    return 2;
	case '4':
	    return 3;
    }

    return 9;
}

unsigned long long genseed (void)
{
    unsigned int r;
    unsigned long long mult, _seed = 0;

    srand ( time(0) );

    r = rand ();

    //printf ("rand() gave %i\n", r);

    for(mult=1; r; mult*=10)
    {
	_seed += mult * ((r % 4)+1);
	r /= 4;
	//printf ("r is %10i, Seed is %17lli\n", r, seed);
    }

    //printf ("Seed computed: %lli\n", _seed);

    seed = malloc(sizeof(char)*30);
    sprintf (seed, "%lli", _seed);

    return _seed;
}

void dump (short *maze, short *stat)
{
    short i,j;

    for (i=0; i<(H*2); i++)
    {
	for (j=0; j<W; j++)
	{
	    if (i%2) /* odd */
	    {
		if ( *(maze) == 2 || *(maze) == 3)
		    putchar ('|');
		else
		    putchar (' ');

		if (stat)
		{
		    printf ("%3i", *stat);
		    stat++;
		}
		else
		    printf ("   ");

		maze++;
	    }
	    else /* even */
	    {
		putchar ('+');

		if ( *(maze) == 1 || *(maze) == 3)
		    printf ("---");
		else
		    printf ("   ");

		maze++;
	    }
	}

	if ( !(i % 2) )
	    maze -= W;

	putchar ('\n');
    }
}

/* vim: set nowrap nodigraph foldmethod=syntax: */
