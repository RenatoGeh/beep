#ifndef _CMD_SMILEY_HH
#define _CMD_SMILEY_HH

#include <string>
#include <vector>

#include "cmd/command.hh"

namespace cmd {
  class Smiley : public Command {
    public:
      Smiley(void) : Command("smiley", "!smiley",
          "This command prints a smiley of given name.", "prints smiley") {}

      std::string Do(const std::vector<std::string> &params);
  };
}

#endif
