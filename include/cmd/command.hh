#ifndef _CMD_COMMAND_HH
#define _CMD_COMMAND_HH

#include <vector>
#include <string>

namespace cmd {
  // Command defines a command.
  class Command {
    public:
      Command(const std::string &n, const std::string &t,
              const std::string &h, const std::string &d) :
        name(n), tag(t), help(h), desc(d) {}
      Command(void) = delete;
      virtual ~Command(void) {}

      const std::string& Name(void) { return name; }
      const std::string& Tag(void) { return tag; }
      const std::string& Help(void) { return help; }
      const std::string& Description(void) { return desc; }

      virtual std::string Do(const std::vector<std::string> &params) = 0;

    protected:
      std::string name;
      std::string tag;
      std::string help;
      std::string desc;
  };
}

#endif
