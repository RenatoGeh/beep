#include <cstdio>

#include <array>
#include <string>

#include "libircclient.h"
#include "libirc_rfcnumeric.h"

#include "bot.hh"
#include "backlog.hh"
#include "utils.hh"
#include "io.hh"

#include "cmd/listener.hh"
#include "cmd/help.hh"
#include "cmd/echo.hh"
#include "cmd/leave.hh"

#include "capivara.hh"

Capivara::Capivara(void) : Bot("capivara") {
  io::Mkdir("capivara");
  logs = new Backlog("capivara/backlog.txt", 100);
  debug = new InstantLog("capivara/debug.txt");
  leave = new cmd::Leave();
  cmd::Help *h = new cmd::Help({new cmd::Echo(), leave});
  listener = new cmd::Listener(h->Commands());
}

Capivara::~Capivara(void) {
  delete logs;
  delete debug;
  delete listener;
}

int Capivara::Say(std::string msg, std::string channel) {
  debug->Log(name + " -> " + channel, msg);
  return Bot::Say(msg, channel);
}

int Capivara::Whisper(std::string msg, std::string user) {
  debug->Log(name + " -> " + user, msg);
  return Bot::Whisper(msg, user);
}

void Capivara::OnConnect(void) {
  if (Join("#capivara_test")) return;
  Broadcast("Hi! I'm a capivara, and I'm a bot. See my code at (https://github.com/renatogeh/beep).");
  Broadcast("Use !help for a list of available commands.");
}

void Capivara::OnMessage(std::string user, std::string channel, std::string text) {
  logs->Log(user, text);
  printf("User: %s, Channel: %s, Text: %s\n", user.c_str(), channel.c_str(), text.c_str());
  Say(listener->Listen(user, channel, text), channel);
  if (leave->Check()) Disconnect();
}

void Capivara::OnNumeric(std::string origin, std::vector<std::string> &params) {}
void Capivara::OnQuit(std::string user, std::string reason) {}
void Capivara::OnJoin(std::string user, std::string channel) {}
void Capivara::OnPart(std::string user, std::string channel, std::string reason) {}
void Capivara::OnPrivate(std::string user, std::string nick, std::string text) {}
