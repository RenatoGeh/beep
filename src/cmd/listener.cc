#include <cstdio>

#include <string>
#include <initializer_list>

#include "utils.hh"
#include "cmd/command.hh"

#include "cmd/listener.hh"

namespace cmd {
  Listener::~Listener(void) {
    for (auto it = cmds.begin(); it != cmds.end(); ++it)
      delete *it;
  }

  std::string Listener::Listen(const std::string &user, const std::string &channel,
      const std::string &text) {
    std::vector<std::string> params = utils::Tokenize(text);
    params.insert(params.begin(), channel);
    params.insert(params.begin(), utils::StripUsername(user));
    for (auto it = cmds.begin(); it != cmds.end(); ++it) {
      Command *c = *it;
      if (c->Tag() == params[2]) {
        printf("Found match: %s == %s\n", c->Tag().c_str(), params[2].c_str());
        return c->Do(params);
      }
    }
    puts("Arguments:");
    for (auto it = params.begin(); it != params.end(); ++it)
      printf("  > %s\n", it->c_str());
    return "";
  }
}
