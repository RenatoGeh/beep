#include <string>
#include <vector>

#include "utils.hh"

#include "cmd/echo.hh"

namespace cmd {
  std::string Echo::Do(const std::vector<std::string> &params) {
    std::string msg;
    for (auto it = params.begin()+3; it != params.end(); ++it)
      msg += " " + *it;
    return utils::Sprintf("%s said:%s", params[0], msg);
  }
}
