#include <time.h>
#include <cstdlib>

#include "chamber.h"

Chamber **initChambers(void);
void printIntro();
void gameLoop(Chamber **cave);
int shoot(int shots);
int promptMove(Chamber **cave, int chamber);
bool shootOrRun(Chamber **cave, int *chamber, int *shots);
void moveWampus(Chamber **cave, int chamber);
int bats(Chamber **cave, int chamber);
void pitMsg();
