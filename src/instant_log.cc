#include "utils.hh"
#include "io.hh"
#include "chronos.hh"

#include "instant_log.hh"

InstantLog::InstantLog(std::string filename, int cache_size) : fname(filename), cache(cache_size) {
  fdisk = io::Open(filename, "a+");
}

InstantLog::~InstantLog(void) {
  io::Close(fdisk);
}

unsigned int InstantLog::Log(std::string user, const std::string &text) {
  user.assign(utils::StripUsername(user));
  std::string cnt = " > " + (user + " | ") + text;
  std::string line = chronos::Now() + cnt;
  fdisk->Open();
  fdisk->WriteLine(line);
  fdisk->Close();
  if (cache.size() > _INSTANT_LOG_LOG_CACHE)
    cache.pop_front();
  cache.push_back(cnt);
  return line.size();
}
