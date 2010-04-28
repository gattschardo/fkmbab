#include "wampus.h"

int
main(int argc, char **argv)
{
  Chamber **cave = initChambers();

  printIntro();
  gameLoop(cave);

  for (int i = 0; i < 20; i++) {
    delete cave[i];
  }

  return 0;
}

Chamber **initChambers(void)
{
  static Chamber *cave[20];

  srand(time(0));

  // outer circle
  cave[0] = new Chamber(9, 1, 10);
  for (int i = 1; i < 9; i++)
    cave[i] = new Chamber(i-1, i+1, i+10);
  cave[9] = new Chamber(8, 0, 19);

  // inner circle
  cave[10] = new Chamber(19, 0, 11);
  for (int i = 11; i < 19; i++)
    cave[i] = new Chamber(i-1, i+1, i-10);
  cave[19] = new Chamber(18, 10, 9);

  cave[rand() % 20]->putWampus();
  cave[rand() % 20]->putBats();
  cave[rand() % 20]->putBats();
  cave[rand() % 20]->putBats();
  cave[rand() % 20]->putExit();
  cave[rand() % 20]->makePit();

  return cave;
}

void gameLoop(Chamber **cave)
{
  bool alive = true, won = false;
  int shots = 6;
  int currentCave = rand() % 20;

  while (alive && !won) {
    if (cave[currentCave]->haveWampus())
      alive = shootOrRun(cave, &currentCave, &shots);

    if (cave[currentCave]->haveBats())
      currentCave = bats(cave, currentCave);

    if (cave[currentCave]->havePit()) {
      alive = false;
      pitMsg();
    }

    currentCave = promptMove(cave, currentCave);

    won = cave[currentCave]->haveExit();
  }
    
  if (won)
    std::cout << "Congratulations, you found your way out!" << std::endl;
}

void printIntro()
{
  const char *introString =
  "You are lost in a cave that has\n"
  "20 chambers. All chambers have\n"
  "tunnels to three others. One of\n"
  "the tunnels goes to the mouth of\n"
  "get home free.\n\n"
  "Wampus lives in the cave. He's\n"
  "a big hairy creature that will\n"
  "gobble you if he gets you. He\n"
  "will run to another chamber if\n"
  "you shoot at him. Your gun only\n"
  "has six shots, however, so use\n"
  "them wisely.\n"
  //"       Hit any key to continue.\n"
  "\nSome chambers have bats in them.\n"
  "they scare you so bad you simply\n"
  "run through the nearest tunnel.\n"
  "One chamber has a deep pit --\n"
  "don't fall in or you'll never\n"
  "get home.\n\n";

  std::cout << "Want the story (y or n)? ";
  std::cout.flush();
  char c;
  std::cin >> c;
  if (c == 'y')
    std::cout << introString;
}

int shoot(int shots)
{
  if (shots < 1)
    return 0;

  std::cout << "BANG ... he ran out." << std::endl;

  return shots-1;
}

int promptMove(Chamber **cave, int chamber)
{
  int exits[3];
  bool nearExit = false;
  
  for (int i = 0; i < 3; i++) {
    exits[i] = cave[chamber]->getExit(i);
    if (cave[exits[i]]->haveExit())
      nearExit = true;
  }

  std::cout << "You are in room " << chamber + 1 << std::endl;
  std::cout << "Exits are: " << exits[0] + 1 << ' '
      << exits[1] + 1 << ' '
      << exits[2] + 1 << std::endl;
  if (nearExit)
    std::cout << "You can feel a draft" << std::endl;

  std::cout << "Move to? ";
  std::cout.flush();
  int to;
  std::cin >> to;
  //std::cout << "read " << to << std::endl;

  return (--to == exits[0] || to == exits[1] || to == exits[2]) ?
      to : promptMove(cave, chamber);
}

bool shootOrRun(Chamber **cave, int *chamber, int *shots)
{
  if (*shots < 1) {
    std::cout << "You're out of shots, and\nWampus has you.\n";
    return false;
  }

  std::cout << "Wampus is in this room." << std::endl
      << "Want to shoot or run (s or r) ";
  std::cout.flush();
  char c;
  std::cin >> c;
  
  bool result;
  if (c == 's') {
    *shots = shoot(*shots);
    moveWampus(cave, *chamber);
    result = true;
  } else if (c == 'r') {
    *chamber = cave[*chamber]->getExit(0);
  } else {
    std::cout << "Answer quickly!" << std::endl;
    result = shootOrRun(cave, chamber, shots);
  }

  return result;
}

void moveWampus(Chamber **cave, int chamber)
{
  cave[chamber]->putWampus(false);
  if (cave[cave[chamber]->getExit(1)]->haveExit())
    cave[cave[chamber]->getExit(0)]->putWampus();
  else
    cave[cave[chamber]->getExit(1)]->putWampus();
}

int
bats(Chamber **cave, int chamber)
{
  int next = cave[chamber]->getExit();

  std::cout << "There are bats in here, so" << std::endl
      << "you run to " << next + 1 << std::endl;

  return next;
}

void
pitMsg()
{
  std::cout << "You fell into the pit," << std::endl
      << "so long ..." << std::endl;
}
