#ifndef _CMD_LISTENER_HH
#define _CMD_LISTENER_HH

#include <string>
#include <initializer_list>

#include "cmd/command.hh"

#include "cmd/listener.hh"

namespace cmd {
  // Listener is a listener of commands.
  class Listener {
    public:
      Listener(std::initializer_list<Command*> C) : cmds(C) {}
      Listener(const std::vector<Command*> &C) : cmds(C) {}
      Listener(void) = delete;

      ~Listener(void);

      // Register adds a new command to this listener.
      void Register(Command* c) { cmds.push_back(c); }

      // Register takes an std::vector<Command*> and copies all contents from it to this Listener's
      // list of listened commands.
      void Register(const std::vector<Command*> &l) { cmds.insert(cmds.begin(), l.begin(), l.end()); }

      // Listen listens for a command, taking arguments as the same parameters as Bot::OnMessage. It
      // then executes the given command (if valid) and returns its output. If no commands are found,
      // returns an empty string.
      std::string Listen(const std::string &user, const std::string &channel, const std::string &text);

      const std::vector<Command*>& Commands(void) { return cmds; }
    private:
      std::vector<Command*> cmds;
  };
}

#endif
