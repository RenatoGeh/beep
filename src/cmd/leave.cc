#include <string>
#include <vector>

#include "utils.hh"
#include "cmd/command.hh"

#include "cmd/leave.hh"

namespace cmd {
  std::string Leave::Do(const std::vector<std::string> &params) {
    trigger = true;
    return utils::Sprintf("User %s asked me to leave. :(", params[0]);
  }
}

