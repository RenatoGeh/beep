#ifndef _CAPIVARA_HH
#define _CAPIVARA_HH

#include <string>

#include "bot.hh"
#include "io.hh"
#include "backlog.hh"
#include "userbase.hh"
#include "cmd/listener.hh"
#include "cmd/leave.hh"

class Capivara : public Bot {
  public:
    void OnConnect(void);
    void OnMessage(std::string user, std::string channel, std::string text);
    void OnNumeric(std::string origin, std::vector<std::string> &params);
    void OnQuit(std::string user, std::string reason);
    void OnJoin(std::string user, std::string channel);
    void OnPart(std::string user, std::string channel, std::string reason);
    void OnPrivate(std::string user, std::string nick, std::string text);

    int Say(std::string msg, std::string channel);
    int Whisper(std::string msg, std::string user);

    int Connect(void) { return Bot::Connect("irc.freenode.net"); }

    Capivara(void);
    ~Capivara(void);

  private:
    Backlog *logs;
    InstantLog *debug;
    UserBase *db;

    cmd::Listener *listener;
    cmd::Leave *leave;
};
#endif
