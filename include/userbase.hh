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
    // Pull returns the user associated with the given username. Returns nullptr if not found.
    User* Pull(std::string user) { return lookup_user(user); }
    // Register registers a user. If user is already registered, noop. Parameter ow defines if
    // Register should overwrite and simply create a new User.
    User* Register(std::string user, bool ow = false);
    // Joined warns the UserBase that someone joined the channel. If user is not yet registered,
    // register.
    bool Joined(std::string user);
    // Parted warns the UserBase that someone left the channel.
    bool Parted(std::string user);

  private:
    std::vector<User*> db;
    std::string dir;

    // lookup_user looks up for a user. Returns nullptr if no use with given name is found.
    User* lookup_user(std::string user);
};

#endif
