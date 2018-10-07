#include <string>

#include "utils.hh"
#include "cmd/command.hh"

#include "cmd/help.hh"

namespace cmd {
  std::string Help::Do(const std::vector<std::string> &params) {
    if (params.size() == 3) {
      std::string out = prep + "The following is a list of available commands:";
      for (auto it = cmds.begin(); it != cmds.end(); ++it) {
        Command *c = *it;
        out.append(" " + c->Tag());
      }
      return out;
    }

    std::string f = params[3];
    for (auto it = cmds.begin(); it != cmds.end(); ++it) {
      Command *c = *it;
      if (c->Tag() == f) return c->Help();
    }
    return utils::Sprintf("Command [%s] does not exist.", f);
  }
}
