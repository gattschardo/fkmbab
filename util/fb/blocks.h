/* blocks.h
 *
 * The Block interface for fbtetris and fbhashira
 *
 * written by gattschardo, April 2009
 */

#ifndef BLOCKS_H
#define BLOCKS_H

#include <stdbool.h>

#include "ftk.h"

typedef struct block {
    int xpos, ypos;
    bool falling;
    int color;
    /*void (*create) (int x, int y);
    void (*fall) (void);
    void (*drop) (void);*/
} block;

typedef struct tbgroup {
    int xpos, ypos;
    bool falling;
    /*void (*create) (int x, int y);
    void (*fall) (void);
    void (*drop) (void);*/
    block blocks[4];
} tbgroup;

void setblocklen (int len);
void setbgcolor (int color);
void setgroundline (int pos);

int getblocklen (void);

block *blockcreate (int color, int x, int y);
void blockredraw (block *b);
void blockclear (block *b);
void blockremove (block *victim);

void blockfall (block *blck);

tbgroup *tetris_gcreate (int type, int x, int y);

#endif /* BLOCKS_H */
