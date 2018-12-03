#include <string>
#include <vector>

#include "utils.hh"
#include "userbase.hh"

#include "cmd/register.hh"

namespace cmd {
  std::string Register::Do(const std::vector<std::string> &params) {
    if (params.size() < 4)
      return "";
    std::string uname(params.at(3));
    User *u = db->Pull(uname);
    if (u == nullptr) {
      db->Register(uname, true);
      return utils::Sprintf("User '%s' registered successfully", uname.c_str());
    }
    return utils::Sprintf("User '%s' already registered", uname.c_str());
  }
}
