/*
 * ripoff.c
 *
 * The algorythm used for this generator is a complete ripoff of the generator
 * used in "textmaze" (written by Kirk Baucom).
 *
 * stolen by gattschardo, march 2009
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define H 10
#define W 10

enum walls { NONE=0, UP, LEFT, BOTH };
enum directions { NORTH=0, EAST, SOUTH, WEST };

typedef struct freaklist {
    short *cell;
    struct freaklist *next;
    struct freaklist *prev;
} freaklist;

void connect (freaklist **freakfield, int i, int j);
short *genmaze (short *maze);
void dump (const short *maze, const short *stat);
void freefreaklist (freaklist *first);
void check (short *maze);

int main (int argc, char *argv[])
{
    int i, size = H*W;
    short maze[size];
    char *filename;
    FILE * output;

    if (argc==2)
	    filename = argv[1];
    else
	    filename = "default_ripoff";

    for (i=0; i<size; i++)
	    maze[i] = BOTH;
    //memset (maze, BOTH, size); // FUCKING SHIT?!?!?!?

    srand48 ( time (NULL) );

    puts ("Generating maze...");
    genmaze (maze);

    puts ("Checking it...");
    check (maze);

    puts ("Printing it:");
    dump (maze,0);

    printf ("Dumping it to file \"%s\" ...\n",filename);
    if (!(output = fopen(filename,"w")))
    {
	    perror("fopen");
	    exit(1);
    }
    for (i=0; i<size; i++)
	    fprintf(output,"%1i",maze[i]);
    fputs("\n",output);
    if(fclose(output))
    {
	    perror("fclose");
	    exit(1);
    }

    puts ("All done, leaving!");
    return EXIT_SUCCESS;
}

short *genmaze (short *maze)
{
    int i, dir, connects = 0,
	size = H*W;
    short setfield[size];
    freaklist *freakfield[size];

    for(i=0;i<size;i++)
    {
	setfield[i] = i;
	freakfield[i] = malloc (sizeof(freaklist));
	freakfield[i]->cell = &setfield[i];
	freakfield[i]->next = NULL;
	freakfield[i]->prev = NULL;
    }

    while (connects < size-H-W)
    {
	i = lrand48 ()%size;
	if(*(maze+i) == NONE)
		continue;
	if( (i % W == W-1) || ( i >= size-W) )
		continue;

	dir = lrand48 ()%4;

	fprintf(stderr,"%i connect(s), working on field %i, set %i (%i), direction %i\n",
			connects, i, setfield[i], *(maze+i), dir);
	
	switch((enum directions) dir)
	{
	    case NORTH:
		if (i>=W && (setfield[i] != setfield[i-W]) )
		{
		    switch((enum walls) *(maze+i))
		    {
			case BOTH:
			    *(maze+i) = LEFT;
			    connect (freakfield, i, i-W);
			    connects++;
			    break;
			case UP:
			    *(maze+i) = NONE;
			    connect (freakfield, i, i-W);
			    connects++;
			    break;
			case LEFT:
			    fprintf(stderr,"North isn't possible\n");
			    break;
			case NONE:
			    fprintf(stderr,"NONE, that's ");
			default:
			    fprintf(stderr,"Weird: %i %i\n",i,*(maze+i));
		    }
		}
		else
			fprintf(stderr,"Connect not possible\n");
		break;
	    case EAST:
		if ( (i % W != W-2) && (setfield[i] != setfield[i+1]) )
		{
		    switch((enum walls) *(maze+i+1))
		    {
			case BOTH:
			    *(maze+i+1) = UP;
			    connect (freakfield, i, i+1);
			    connects++;
			    break;
			case LEFT:
			    *(maze+i+1) = NONE;
			    connect (freakfield, i, i+1);
			    connects++;
			    break;
			case UP:
			    fprintf(stderr,"East isn't possible\n");
			    break;
			case NONE:
			    fprintf(stderr,"NONE, that's ");
			default:
			    fprintf(stderr,"Weird: %i %i\n",i,*(maze+i));
		    }
		}
		else
			fprintf(stderr,"Connect not possible\n");
		break;
	    case SOUTH:
		if ( i < size-2*W && (setfield[i] != setfield[i+W]) )
		{
		    switch((enum walls) *(maze+i+W))
		    {
			case BOTH:
			    *(maze+i+W) = LEFT;
			    connect (freakfield, i, i+W);
			    connects++;
			    break;
			case UP:
			    *(maze+i+W) = NONE;
			    connect (freakfield, i, i+W);
			    connects++;
			    break;
			case LEFT:
			    fprintf(stderr,"South isn't possible\n");
			    break;
			case NONE:
			    fprintf(stderr,"NONE, that's ");
			default:
			    fprintf(stderr,"Weird: %i %i\n",i,*(maze+i));
		    }
		}
		else
			fprintf(stderr,"Connect not possible\n");
		break;
	    case WEST:
		if ( i % W != 0 && (setfield[i] != setfield[i-1]) )
		{
		    switch((enum walls) *(maze+i))
		    {
			case BOTH:
			    *(maze+i) = UP;
			    connect (freakfield, i, i-1);
			    connects++;
			    break;
			case LEFT:
			    *(maze+i) = NONE;
			    connect (freakfield, i, i-1);
			    connects++;
			    break;
			case UP:
			    fprintf(stderr,"West isn't possible\n");
			    break;
			case NONE:
			    fprintf(stderr,"NONE, that's ");
			default:
			    fprintf(stderr,"Weird: %i %i\n",i,*(maze+i));
		    }
		}
		else
			fprintf(stderr,"Connect not possible\n");
		break;
	    default:
		fprintf(stderr,"lrand48() is an asshole!\n");
	}
    }

    //dump (maze, setfield);

    freefreaklist (freakfield[0]);

    return maze;
}

void connect (freaklist *freakfield[], int i, int j)
{
    //fprintf(stderr,"Hello!\n");
    fprintf(stderr,"Connecting set %2i and %2i\n",*freakfield[i]->cell,*freakfield[j]->cell);

    short *cell1 = freakfield[i]->cell,
	  *cell2 = freakfield[j]->cell;
    freaklist *ptr,
	      *p_first;

    if (*cell1 == *cell2)
    {
	fprintf(stderr,"Already connected, connect aborted\n");
	return;
    }
    else if (*cell1 < *cell2)
    {
	for (ptr=freakfield[j]; ptr; ptr = ptr->prev)
	{
	    *ptr->cell = *cell1;
	    p_first = ptr;
	    if(ptr!=freakfield[j])
		    fprintf(stderr,"Went back\n");
	}
	//fprintf(stderr,"p_first is %p, prev %p\n",p_first,p_first->prev);

	for (ptr=freakfield[j]; ptr; ptr = ptr->next)
	{
	    *ptr->cell = *cell1;
	    if(ptr!=freakfield[j])
		    fprintf(stderr,"Went forward\n");
	}

	for (ptr=freakfield[i]; ptr->next; ptr =ptr->next)
	    continue;

	ptr->next = p_first;
	ptr->next->prev = ptr;

	//freakfield[j] = freakfield[i];
    }
    else if (*cell1 > *cell2)
    {
	for (ptr=freakfield[i]; ptr; ptr = ptr->prev)
	{
	    *ptr->cell = *cell2;
	    p_first = ptr;
	    if(ptr!=freakfield[i])
		    fprintf(stderr,"Went back\n");
	}

	for (ptr=freakfield[i]; ptr; ptr = ptr->next)
	{
	    *ptr->cell = *cell2;
	    if(ptr!=freakfield[i])
		    fprintf(stderr,"Went forward\n");
	}

	for (ptr=freakfield[j]; ptr->next; ptr =ptr->next)
	    continue;

	ptr->next = p_first;
	ptr->next->prev = ptr;

	//freakfield[i] = freakfield[j];
    }
    else
	    fprintf(stderr,"You must be kiddin' me!\n");

    //fprintf(stderr,"Connect successful!\n");
}

void freefreaklist (freaklist *first)
{
    freaklist *prev;

    for(prev=first; first; first = first->next)
    {
	prev=first;
	free (prev);
    }
}

void dump (const short *maze, const short *stat)
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
		    printf ("  ");

		maze++;
	    }
	    else /* even */
	    {
		putchar ('+');

		if ( *(maze) == 1 || *(maze) == 3)
		    printf ("--");
		else
		    printf ("  ");

		maze++;
	    }
	}

	if ( !(i % 2) )
	    maze -= W;

	putchar ('\n');
    }
}

void check (short *maze)
{
    short i;

    for (i=W-1;i<W*H;i+=W)
    {
	if (*(maze+i) == BOTH)
	    *(maze+i) = LEFT;
	else
	{
	    printf ("Error at field %i",i);
	    exit (EXIT_FAILURE);
	}
    }

    for (i=W*H-H;i<W*H-1;i++)
    {
	if (*(maze+i) == BOTH)
	    *(maze+i) = UP;
	else
	{
	    printf ("Error at field %i",i);
	    exit (EXIT_FAILURE);
	}
    }

    if (*(maze+H*W-1) == LEFT)
	    *(maze+H*W-1) = NONE;
    else
    {
	    printf ("Error at field %i",H*W-1);
	    exit (EXIT_FAILURE);
    }
}

/* vim: set nowrap nodigraph foldmethod=syntax: */
