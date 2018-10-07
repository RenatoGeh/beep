#ifndef _CMD_LEAVE_HH
#define _CMD_LEAVE_HH

#include <string>
#include <vector>

#include "cmd/command.hh"

namespace cmd {
  class Leave : public Command {
    public:
      Leave(void) : Command("leave", "!leave", "This command forces the bot to disconnect.",
          "disconnects the bot"), trigger(false) {}
      ~Leave(void) {}

      std::string Do(const std::vector<std::string> &params);

      bool Check(void) { return trigger; }
    private:
      bool trigger;
  };
}

#endif
