#include <array>
#include <string>

#include "libircclient.h"
#include "libirc_rfcnumeric.h"

#include "bot.hh"
#include "backlog.hh"
#include "utils.hh"
#include "io.hh"

#include "capivara.hh"

Capivara::Capivara(void) : Bot("capivara") {
  io::Mkdir("capivara");
  logs = new Backlog("capivara/backlog.txt", 100);
}

Capivara::~Capivara(void) {
  delete logs;
}

void Capivara::OnConnect(void) {
}

void Capivara::OnMessage(std::string user, std::string channel, std::string text) {
  logs->Log(user, text);
}

void Capivara::OnNumeric(std::string origin, std::vector<std::string> &params) {}
void Capivara::OnQuit(std::string user, std::string reason) {}
void Capivara::OnJoin(std::string user, std::string channel) {}
void Capivara::OnPart(std::string user, std::string channel, std::string reason) {}
void Capivara::OnPrivate(std::string user, std::string nick, std::string text) {}
