#include <iostream>

class Chamber {

private:
  bool isPit;
  bool hasBats;
  bool hasExit;
  bool hasWampus;
  int connects[3];

public:
  Chamber(int a, int b, int c, bool hasBats = false, bool hasExit = false,
       bool hasWampus = false, bool isPit = false);

  void print();

  void putBats(bool b = true);
  void putExit();
  void putWampus(bool b = true);
  void makePit();

  bool haveExit();
  bool haveWampus();
  bool haveBats();
  bool havePit();

  int getExit(int i = 0);
};
