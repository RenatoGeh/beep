#ifndef _CMD_LEAVE_HH
#define _CMD_LEAVE_HH

#include <string>
#include <vector>

#include "cmd/command.hh"

#ifndef _CMD_LEAVE_KEY_SIZE
#define _CMD_LEAVE_KEY_SIZE 6
#endif

namespace cmd {
  class Leave : public Command {
    public:
      Leave(void);
      ~Leave(void) {}

      std::string Do(const std::vector<std::string> &params);

      bool Check(void) { return trigger; }
    private:
      bool trigger;
      char pw[_CMD_LEAVE_KEY_SIZE];
  };
}

#endif
