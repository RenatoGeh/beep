#ifndef _CMD_ECHO_HH
#define _CMD_ECHO_CC

#include <string>

#include "cmd/command.hh"

namespace cmd {
  class Echo : public Command {
    public:
      Echo(void) : Command("echo", "!echo",
          "This command echoes (i.e. repeats) whatever you say.", "echoes a message") {}

      std::string Do(const std::vector<std::string> &params);
  };
}

#endif
