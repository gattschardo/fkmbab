/* hashlogic.h */

/* some general defines */
#define BLOCKSIZE 10
#define DEFAULT_DELAY 8000
#define DEF_W 6
#define DEF_H 12

/* definition of colors */
#define COL_BG COL_BLACK
#define C1 COL_RED
#define C2 COL_GREEN
#define C3 COL_BLUE
#define C4 COL_YELLOW
#define C5 COL_PURPLE
#define C6 COL_ORANGE

int drop (void);
void letitfall (int howmany); /* DIY */
int calcndrop (void);
void killblocks (int color); /* DIY */

bool check_quit ();
int check_line (int x, int y);
int check_col (int x, int y);
int check_diagdown (int x, int y);
int check_diagup (int x, int y);

void settimer (unsigned const int delay);
int gettimer (void);
