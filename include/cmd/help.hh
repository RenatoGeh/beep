#ifndef _CMD_HELP_HH
#define _CMD_HELP_HH

#include <string>
#include <initializer_list>

#include "cmd/command.hh"

namespace cmd {
  class Help : public Command {
    public:
      Help(const std::string &p, std::initializer_list<Command*> C) :
        Command("help", "!help",
            "This command shows info on available commands and their usage.",
            "shows this help message"), prep(p), cmds(C) { cmds.push_back(this); }
      Help(std::initializer_list<Command*> C) : Help("", C) {}

      void Register(Command *c) { cmds.push_back(c); }

      std::string Do(const std::vector<std::string> &params);

      const std::vector<Command*>& Commands(void) { return cmds; }
    private:
      std::string prep;
      std::vector<Command*> cmds;
  };
}

#endif
