#include <cstdio>

#include <string>

#include "libircclient.h"
#include "libirc_rfcnumeric.h"

#include "bot.hh"
#include "echo.hh"

void EchoBot::OnConnect(void) {
  std::string channel("#ccsl-usp");
  Join(channel);
  Say("Successfully joined channel.", channel);
  Say("Testing echo message.", channel);
  Say("Use '!echo message' to echo your message.", channel);
  Say("Use '!leave' to make me leave. :(", channel);
}

void EchoBot::OnMessage(std::string user, std::string channel, std::string text) {
  auto cnt_it = text.find(" ");
  std::string prefix = text.substr(0, cnt_it);
  if (prefix == "!leave") {
    Say("Goodbye. :)", "#ccsl-usp");
    Disconnect();
  } else if (prefix == "!echo") {
    std::string echo(text);
    char buf[256];
    sprintf(buf, "User %s said: ", user.c_str());
    Say(std::string(buf).append(text.substr(cnt_it)), "#ccsl-usp");
  } else {
    Say("I did not understand you. Try '!echo' or '!leave'.", "#ccsl-usp");
  }
}


