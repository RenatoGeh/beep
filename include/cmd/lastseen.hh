#ifndef _CMD_LASTSEEN_HH
#define _CMD_LASTSEEN_HH

#include <string>
#include <vector>

#include "userbase.hh"
#include "cmd/command.hh"

namespace cmd {
  class LastSeen : public Command {
    public:
      LastSeen(UserBase *udb) : Command("lastseen", "!lastseen",
          "This command shows the last time this bot saw a user.", "prints last seen user info"),
      db(udb) {}

      std::string Do(const std::vector<std::string> &params);
    private:
      UserBase *db;
  };
}

#endif
