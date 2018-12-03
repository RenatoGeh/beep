#ifndef _USER_HH
#define _USER_HH

#include <string>
#include <vector>

#include "io.hh"
#include "chronos.hh"
#include "instant_log.hh"

class User : protected InstantLog {
  public:
    User(std::string uname, std::string dir);
    ~User(void);

    const std::string& Name(void) { return name; }
    const std::string& Filepath(void) { return fname; }
    const std::vector<std::string>* Quotes(void) { return &quotes; }

    void Login(void);
    void Logout(void);
    void Initialize(void);

    unsigned int Log(std::string user, const std::string &text);

    bool Logged(void) { return logged; }

    std::string LastSeen(void) {
      chronos::Time t;
      return chronos::DiffString(last_seen, t);
    }

  private:
    std::string name;
    bool logged;

    std::vector<std::string> quotes;

    chronos::Time last_seen;

    std::string setup(const std::string &uname, const std::string &dir);
};

#endif
