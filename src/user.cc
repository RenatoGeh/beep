#include <string>

#include "io.hh"

#include "user.hh"

std::string User::setup(const std::string &uname, const std::string &dir) {
  io::Mkdir(dir);
  return dir + "/" + uname + ".log";
}

User::User(std::string uname, std::string dir) : InstantLog(setup(uname, dir)),
  name(uname), logged(false) {}

User::~User(void) {
  io::Close(fdisk);
}

void User::Login(void) {
  if (fdisk->Empty())
    Initialize();
  fdisk->Open("r");
  std::string line;
  int n = fdisk->Read(&line);
  last_seen.Import(line);
  const int END = std::char_traits<char>::eof();
  while (n != END) {
    n = fdisk->Read(&line);
    if (n == END) break;
    quotes.push_back(line);
  }
  fdisk->Close();
}

void User::Logout(void) {
  last_seen.Reset();
  fdisk->Open("w");
  fdisk->WriteLine(last_seen.Export());
  fdisk->Close();
  quotes.clear();
}

void User::Initialize(void) {
  last_seen.Reset();
  fdisk->Open("w");
  fdisk->WriteLine(last_seen.Export());
  fdisk->Close();
}

unsigned int User::Log(std::string user, const std::string &text) {
  if (logged) return 0;
  return InstantLog::Log(user, text);
}
