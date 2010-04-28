#include "chamber.h"

Chamber::Chamber(int a, int b, int c, bool hasBats, bool hasExit,
       bool hasWampus, bool isPit)
{
  connects[0] = a;
  connects[1] = b;
  connects[2] = c;

  this->isPit = isPit;
  this->hasBats = hasBats;
  this->hasExit = hasExit;
  this->hasWampus = hasWampus;
}

void
Chamber::print()
{
  std::cout << "Chamber connected to";
  for (int i = 0; i < 3; i++)
    std::cout << " " << connects[i];
  std::cout << "\n";
}

void
Chamber::putBats(bool b)
{
  hasBats = true;
}

void
Chamber::putExit()
{
  hasExit = true;
}

void
Chamber::putWampus(bool b)
{
  hasWampus = true;
}

void
Chamber::makePit()
{
  isPit = true;
}

int
Chamber::getExit(int i)
{
  if (i < 0 || i > 3)
    return -1;

  return connects[i];
}

bool
Chamber::haveExit()
{
  return hasExit;
}

bool
Chamber::haveWampus()
{
  return hasWampus;
}

bool
Chamber::haveBats()
{
  return hasBats;
}

bool
Chamber::havePit()
{
  return isPit;
}
