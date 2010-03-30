/*
 * dfuncs.h
 *
 * drawing functions & helper functions
 */

#ifdef NCURSES
# include <ncurses.h>
#endif

#include <stdbool.h>

#ifdef STDIO
# include <stdio.h>
#endif

extern short *maze, **pos, width;
extern int dir, maxsight, endsteph, endstepw;

#ifdef NCURSES
extern int c, l;
#endif /* NCURSES */

/* positions are:
 * 0 left
 * 1 center
 * 2 right
 */

/* functions */
bool mazemove (short **pos, int direction);
bool wall (int step, int position);
int findmaxsight (int cols, int rows);
int findstopwall (int step, int position);

/* procedures */
/* mode: 0: clear all, 1: draw normal */
void onresize (int signal);
void draw (void);
void drawfloor (bool mode);
void drawposts (bool mode, int step);
void drawstopwall (bool mode, int step, int distance, int position);
void drawwall (bool mode, int step, int position);
void testdraw (bool mode);

#ifdef NCURSES
void statusline (const char *string);
#endif

/* vim: set nodigraph foldmethod=syntax: */
