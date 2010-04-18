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

void Chamber::print(void)
  {
    std::cout << "Chamber\n";
  }
