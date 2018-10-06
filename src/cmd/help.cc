#include <string>

#include "utils.hh"
#include "cmd/command.hh"

#include "cmd/help.hh"

namespace cmd {
  std::string Help::Do(const std::vector<std::string> &params) {
    if (params.size() == 0) {
      std::string out;
      out.append(prep + "\nThe following is a list of available commands:\n");
      for (auto it = cmds.begin(); it != cmds.end(); ++it) {
        Command *c = *it;
        out.append("  " + c->Name() + " - " + c->Description());
      }
      return out;
    }

    std::string f = params[0];
    for (auto it = cmds.begin(); it != cmds.end(); ++it) {
      Command *c = *it;
      if (c->Name() == f) return c->Help();
    }
    return utils::Sprintf("Command [%s] does not exist.", f);
  }
}
