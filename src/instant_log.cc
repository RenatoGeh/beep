#include "utils.hh"
#include "io.hh"
#include "chronos.hh"

#include "instant_log.hh"

InstantLog::InstantLog(std::string filename) : fname(filename) {
  fdisk = io::Open(filename, "a+");
}

InstantLog::~InstantLog(void) {
  io::Close(fdisk);
}

unsigned int InstantLog::Log(std::string user, const std::string &text) {
  user.assign(utils::StripUsername(user));
  std::string line = chronos::Now() + " > " + (user + " | ") + text;
  fdisk->Open();
  fdisk->WriteLine(line);
  fdisk->Close();
  return line.size();
}
