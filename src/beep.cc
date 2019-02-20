#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "bot.hh"
#include "capivara.hh"

int main(int argc, char *args[]) {
  Capivara *b;
  if (argc == 4)
    b = new Capivara(args[1], args[2], args[3]);
  else if (argc == 3)
    b = new Capivara(args[1], args[2]);
  else
    b = new Capivara();

  Bot::Assign(b);

  puts("Connecting...");
  b->Connect();
  puts("Starting...");
  b->Start();
  puts("Goodbye.");

  delete b;

  return 0;
}
