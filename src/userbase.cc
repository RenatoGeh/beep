#include <string>
#include <vector>

#include "user.hh"
#include "io.hh"

#include "userbase.hh"

UserBase::UserBase(std::string dirname) : dir(dirname) {
  io::Mkdir(dirname);
}

UserBase::~UserBase(void) {}

const std::vector<std::string>* UserBase::Retrieve(std::string user) {
  for (auto it = db.begin(); it != db.end(); ++it) {
    User *u = *it;
    if (user == u->Name())
      return u->Quotes();
  }
  return nullptr;
}

User* UserBase::lookup_user(std::string user) {
  for (auto it = db.begin(); it != db.end(); ++it) {
    User *u = *it;
    if (user == u->Name()) return u;
  }
  return nullptr;
}

User* UserBase::Register(std::string user, bool ow) {
  User *u;
  if (!ow) {
    u = lookup_user(user);
    if (u != nullptr)
      return u;
  }
  u = new User(user, dir);
  db.push_back(u);
  return u;
}

bool UserBase::Joined(std::string user) {
  bool rval = true;
  User *u = lookup_user(user);
  if (u == nullptr) {
    u = Register(user, true);
    rval = false;
  }
  u->Login();
  return rval;
}

bool UserBase::Parted(std::string user) {
  bool rval = true;
  User *u = lookup_user(user);
  if (u == nullptr) {
    u = Register(user, true);
    rval = false;
  }
  u->Logout();
  return rval;
}
