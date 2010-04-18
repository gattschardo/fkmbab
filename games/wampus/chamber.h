#include <iostream>

class Chamber {
  bool isPit;
  bool hasBats;
  bool hasExit;
  bool hasWampus;
  int connects[3];

  public:
  Chamber(int a, int b, int c, bool hasBats = false, bool hasExit = false,
       bool hasWampus = false, bool isPit = false);

  void print(void);
};
