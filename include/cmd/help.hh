#ifndef _CMD_HELP_HH
#define _CMD_HELP_HH

#include <string>
#include <initializer_list>

#include "cmd/command.hh"

namespace cmd {
  class Help : protected Command {
    public:
      Help(const std::string &p, std::initializer_list<Command*> C) :
        Command("help", "!help",
            "This command shows info on available commands and their usage.",
            "shows this help message"), prep(p), cmds(C) {}

      std::string Do(const std::vector<std::string> &params);
    private:
      std::string prep;
      std::vector<Command*> cmds;
  };
}

#endif
