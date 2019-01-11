#ifndef _CMD_SUBSTITUTE_HH
#define _CMD_SUBSTITUTE_HH

#include <string>
#include <vector>
#include <regex>

#include "instant_log.hh"
#include "cmd/command.hh"

namespace cmd {
  class Substitute : public Command {
    public:
      Substitute(InstantLog *l) : Command("s foo/bar", "!s",
          "This command replaces a regex occurence of foo with bar.", "replace regex occurences"),
          log(l) {}

      std::string Do(const std::vector<std::string> &params);

    private:
      InstantLog *log;
  };
}

#endif
