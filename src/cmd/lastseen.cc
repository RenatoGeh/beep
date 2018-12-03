#include <string>
#include <vector>

#include "utils.hh"
#include "userbase.hh"

#include "cmd/lastseen.hh"

namespace cmd {
  std::string LastSeen::Do(const std::vector<std::string> &params) {
    if (params.size() < 4)
      return "";
    std::string uname(params.at(3));
    User *u = db->Pull(uname);
    if (u == nullptr)
      return utils::Sprintf("No user registered with username '%s'", uname.c_str());
    return u->LastSeen();
  }
}
