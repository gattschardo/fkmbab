#include "chamber.h"

int main(int argc, char **argv)
{
  Chamber *c = new Chamber(2, 3, 4);
  c->print();
  delete c;

  return 0;
}
