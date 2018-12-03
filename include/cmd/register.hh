#ifndef _CMD_REGISTER_HH
#define _CMD_REGISTER_HH

#include <string>
#include <vector>

#include "userbase.hh"
#include "cmd/command.hh"

namespace cmd {
  class Register : public Command {
    public:
      Register(UserBase *udb) : Command("register", "!register",
          "This command registers this user to the bot's database.",
          "adds user to database"), db(udb) {}

    std::string Do(const std::vector<std::string> &params);

    private:
      UserBase *db;
  };
}

#endif
