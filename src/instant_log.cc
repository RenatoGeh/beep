#include "utils.hh"
#include "io.hh"

#include "instant_log.hh"

InstantLog::InstantLog(std::string filename) : fname(filename) {
  fdisk = io::Open(filename, "+a");
}

InstantLog::~InstantLog(void) {
  delete fdisk;
}

unsigned int InstantLog::Log(std::string user, const std::string &text) {
  user.assign(utils::StripUsername(user));
  _log = (user + " | ") + text;
  Flush();
  return 0;
}

void InstantLog::Flush(void) {
  fdisk->Open();
  io::Write(fdisk, _log);
  fdisk->Close();
}
