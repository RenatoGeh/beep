#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "libircclient.h"
#include "libirc_rfcnumeric.h"

#include "bot.hh"
#include "capivara.hh"

int main(void) {
  Capivara b;
  Bot::Assign(&b);

  puts("Connecting...");
  b.Connect();
  puts("Starting...");
  b.Start();
  puts("Goodbye.");

  return 0;
}
