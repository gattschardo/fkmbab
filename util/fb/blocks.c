/* blocks.c
 *
 * The Block interface for fbtetris and fbhashira
 *
 * written by gattschardo, April 2009
 */

#include <stdlib.h>

#include "blocks.h"

static int blocklen, bgcolor, groundline;

void setblocklen (int len)
{
    blocklen = len;
}

void setbgcolor (int color)
{
    bgcolor = color;
}

void setgroundline (int pos)
{
    groundline = pos;
}

int getblocklen (void)
{
    return blocklen;
}

block *blockcreate (int color, int x, int y)
{
    block *newblock = (block *)malloc (sizeof(block));

    newblock->xpos = x;
    newblock->ypos = y;
    newblock->falling = true;
    newblock->color = color;

    blockredraw (newblock);

    return newblock;
}

void blockredraw (block *b)
{
    if (b->ypos > -1)
	ftk_drawrect (DRAW_FILLED, b->xpos, b->ypos, b->xpos+blocklen-1, b->ypos+blocklen-1, b->color);
    else if (b->ypos+blocklen-1 > -1)
	ftk_drawrect (DRAW_FILLED, b->xpos, 0, b->xpos+blocklen-1, b->ypos+blocklen-1, b->color);
}

void blockclear (block *b)
{
    if (b->ypos > -1)
	ftk_drawrect (DRAW_FILLED, b->xpos, b->ypos, b->xpos+blocklen-1, b->ypos+blocklen-1, bgcolor);
    else if (b->ypos+blocklen-1 > 0)
	ftk_drawrect (DRAW_FILLED, b->xpos, 0, b->xpos+blocklen-1, b->ypos+blocklen-1, bgcolor);
}

void blockremove (block *victim)
{
    free (victim);
}

void blockfall (block *blck)
{
    if (!blck->falling)
	return;
    else
	(blck->ypos)++;

    if (blck->ypos > 0)
    {
	ftk_drawhline (blck->xpos, blck->xpos+blocklen-1, blck->ypos-1, bgcolor);
	ftk_drawhline (blck->xpos, blck->xpos+blocklen-1, blck->ypos+blocklen-1, blck->color);
    }
    else
	blockredraw (blck);

    if (blck->ypos+blocklen-1 == groundline)
	blck->falling = false;
}
