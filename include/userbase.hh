#ifndef _USERBASE_HH
#define _USERBASE_HH

#include <string>
#include <vector>

#include "user.hh"

class UserBase {
  public:
    UserBase(std::string dir);
    UserBase(void) = delete;
    ~UserBase(void);

    // Retrieve returns an std::vector<std::string> of messages.
    const std::vector<std::string>* Retrieve(std::string user);
    // Register registers a user. Returns true if already exists, false else.
    bool Register(std::string user);

  private:
    std::vector<User*> db;
    std::string dir;
};

#endif
