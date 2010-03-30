/* ftk.c
 *
 * Framebuffer ToolKit
 *
 * written by gattschardo, April 2009
 */

#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "ftk.h"

static int fbfd, xmax, ymax;
static long screensize;
static char *fbp;

static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;

static struct s_col *colselect (int color);

void ftk_init (void)
{

    fbfd = open ("/dev/fb0", O_RDWR);
    if (fbfd < 0)
    {
	perror ("open");
	exit (EXIT_FAILURE);
    }

    if (ioctl (fbfd, FBIOGET_FSCREENINFO, &finfo) < 0)
    {
	perror ("ioctl");
	exit (EXIT_FAILURE);
    }

    if (ioctl (fbfd, FBIOGET_VSCREENINFO, &vinfo) < 0)
    {
	perror ("ioctl");
	exit (EXIT_FAILURE);
    }

    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    fbp = (char *)mmap (0, screensize, PROT_READ | PROT_WRITE,  MAP_SHARED, fbfd, 0);

    if (fbp == MAP_FAILED)
    {
	perror ("mmap");
	exit (EXIT_FAILURE);
    }
}

void ftk_quit (void)
{
    munmap (fbp, screensize);

    close (fbfd);
}

void ftk_drawrawpix (int x, int y,
	unsigned char red, unsigned char green, unsigned char blue)
{
    //static short int bpp = vinfo.bits_per_pixel/8;

    long location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
	(y+vinfo.yoffset) * finfo.line_length;

    *(fbp + location) = blue;
    *(fbp + location +1) = green;
    *(fbp + location +2) = red;
    *(fbp + location +3) = 0; /* Transparency */

    //fprintf (stderr, "New pixel (#%02x%02x%02x) at %i/%i\n", red, green, blue, x, y);
}

void ftk_drawrawvline (int x, int ymin, int ymax,
	unsigned char red, unsigned char green, unsigned char blue)
{
    int i;

    for (i = ymin; i < ymax+1; i++)
	ftk_drawrawpix (x, i, red, green, blue);

    //fprintf (stderr, "Horizontal line from %i/%i to %i/%i done\n", x, ymin, x, ymax);
}

void ftk_drawrawhline (int xmin, int xmax, int y,
	unsigned char red, unsigned char green, unsigned char blue)
{
    int i;

    for (i = xmin; i < xmax+1; i++)
	ftk_drawrawpix (i, y, red, green, blue);

    //fprintf (stderr, "Vertical line from %i/%i to %i/%i done\n", xmin, y, xmax, y);
}

void ftk_drawrawrect (int mode, int xmin, int ymin, int xmax, int ymax,
	unsigned char red, unsigned char green, unsigned char blue)
{
    int i, j;

    if (mode == DRAW_SKEL)
    {
	ftk_drawrawhline (xmin, xmax, ymin, red, green, blue);
	ftk_drawrawhline (xmin, xmax, ymax, red, green, blue);

	ftk_drawrawvline (xmin, ymin, ymax, red, green, blue);
	ftk_drawrawvline (xmax, ymin, ymax, red, green, blue);
    }
    else if (mode == DRAW_FILLED)
	for (i = ymin; i<ymax+1; i++)
	    ftk_drawrawhline (xmin, xmax, i, red, green, blue);
}

void ftk_drawpix (int x, int y, int color)
{
    struct s_col *colr;
    colr = colselect (color);

    ftk_drawrawpix (x, y, colr->red, colr->green, colr->blue);
}

void ftk_drawvline (int x, int ymin, int ymax, int color)
{
    struct s_col *colr;
    colr = colselect (color);

    ftk_drawrawvline (x, ymin, ymax, colr->red, colr->green, colr->blue);
}

void ftk_drawhline (int xmin, int xmax, int y, int color)
{
    struct s_col *colr;
    colr = colselect (color);

    ftk_drawrawhline (xmin, xmax, y, colr->red, colr->green, colr->blue);
}

void ftk_drawrect (int mode, int xmin, int ymin, int xmax, int ymax, int color)
{
    struct s_col *colr;
    colr = colselect (color);

    ftk_drawrawrect (mode, xmin, ymin, xmax, ymax, colr->red, colr->green, colr->blue);
}

void ftk_getmaxxy (int *x, int *y)
{
    *x = vinfo.xres;
    *y = vinfo.yres;
}

static struct s_col *colselect (int color)
{
    static struct s_col cls;

    switch ((enum colors) color)
    {
	case COL_WHITE:
	    cls.red = 255;
	    cls.green = 255;
	    cls.blue = 255;
	    break;
	case COL_GREY:
	    cls.red = 127;
	    cls.green = 127;
	    cls.blue = 127;
	    break;
	case COL_BLACK:
	    cls.red = 0;
	    cls.green = 0;
	    cls.blue = 0;
	    break;
	case COL_RED:
	    cls.red = 255;
	    cls.green = 0;
	    cls.blue = 0;
	    break;
	case COL_GREEN:
	    cls.red = 0;
	    cls.green = 255;
	    cls.blue = 0;
	    break;
	case COL_BLUE:
	    cls.red = 0;
	    cls.green = 0;
	    cls.blue = 255;
	    break;
	case COL_CYAN:
	    cls.red = 0;
	    cls.green = 255;
	    cls.blue = 255;
	    break;
	case COL_MAGENTA:
	    cls.red = 255;
	    cls.green = 0;
	    cls.blue = 255;
	    break;
	case COL_YELLOW:
	    cls.red = 255;
	    cls.green = 255;
	    cls.blue = 0;
	    break;
	case COL_PURPLE:
	    cls.red = 100;
	    cls.green = 0;
	    cls.blue = 150;
	    break;
	case COL_DGREEN:
	    cls.red = 0;
	    cls.green = 127;
	    cls.blue = 0;
	    break;
	case COL_ORANGE:
	    cls.red = 210;
	    cls.green = 105;
	    cls.blue = 30;
	    break;
	case COL_NONE:
	default:
	    cls.red = 0;
	    cls.green = 0;
	    cls.blue = 0;
    }

    return &cls;
}
