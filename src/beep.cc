#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "libircclient.h"
#include "libirc_rfcnumeric.h"

#include "bot.hh"
#include "echo.hh"

int main(void) {
  EchoBot b("CCSLTestBot");
  Bot::Assign(&b);

  puts("Connecting...");
  b.Connect("irc.freenode.net");
  puts("Starting...");
  b.Start();
  puts("Goodbye.");

  return 0;
}
