#ifndef _ECHO_HH
#define _ECHO_HH

#include <string>

#include "bot.hh"

class EchoBot : public Bot {
  public:
    void OnConnect(void);
    void OnMessage(std::string user, std::string channel, std::string text);

    EchoBot(const char *username) : Bot(username) {}
    EchoBot(void) = delete;
    ~EchoBot(void) {}
};

#endif
