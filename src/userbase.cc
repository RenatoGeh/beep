#include <string>
#include <vector>

#include "user.hh"

#include "userbase.hh"

UserBase::UserBase(std::string dirname) : dir(dirname) {}
UserBase::~UserBase(void) {}

const std::vector<std::string>* UserBase::Retrieve(std::string user) {
  for (auto it = db.begin(); it != db.end(); ++it) {
    User *u = *it;
    if (user == u->Name())
      return u->Quotes();
  }
  return nullptr;
}

bool UserBase::Register(std::string user) {
  for (auto it = db.begin(); it != db.end(); ++it) {
    User *u = *it;
    if (user == u->Name()) return true;
  }
  User *u = new User(user, dir);
  db.push_back(u);
  return false;
}
