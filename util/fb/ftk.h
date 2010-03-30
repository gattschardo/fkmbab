/* ftk.h
 *
 * Framebuffer ToolKit
 *
 * written by gattschardo, April 2009
 */

#ifndef FTK_H
#define FTK_H

enum drawmodes { DRAW_SKEL, DRAW_FILLED };
enum colors { COL_NONE, COL_RED, COL_GREEN, COL_BLUE, COL_YELLOW, COL_MAGENTA,
    COL_BLACK, COL_WHITE, COL_GREY, COL_CYAN, COL_PURPLE, COL_DGREEN, COL_ORANGE };

struct s_col {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

void ftk_init (void);
void ftk_quit (void);

void ftk_getmaxxy (int *x, int *y);

void ftk_drawrawpix (int x, int y, unsigned char red, unsigned char green, unsigned char blue);
void ftk_drawrawvline (int x, int ymin, int ymax,
	unsigned char red, unsigned char green, unsigned char blue);
void ftk_drawrawhline (int xmin, int xmax, int y,
	unsigned char red, unsigned char green, unsigned char blue);
void ftk_drawrawrect (int mode, int xmin, int ymin, int xmax, int ymax,
	unsigned char red, unsigned char green, unsigned char blue);

void ftk_drawpix (int x, int y, int color);
void ftk_drawvline (int x, int ymin, int ymax, int color);
void ftk_drawhline (int xmin, int xmax, int y, int color);
void ftk_drawrect (int mode, int xmin, int ymin, int xmax, int ymax, int color);

#endif /* FTK_H */
