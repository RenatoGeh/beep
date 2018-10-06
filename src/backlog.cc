#include <cstdio>

#include <list>
#include <string>

#include "utils.hh"
#include "io.hh"

#include "backlog.hh"

Backlog::Backlog(std::string filename, unsigned int c) : InstantLog(filename), size(0), cap(c) {}
Backlog::~Backlog(void) {}

unsigned int Backlog::Log(std::string user, const std::string &text) {
  user.assign(utils::StripUsername(user));
  std::string line = (user + " | ") + text;

  printf("LOG: %s\n", line.c_str());
  ++size;
  log.push_back(line);
  if (size > cap) {
    Flush();
  }

  return size;
}

void Backlog::Flush(void) {
  fdisk->Open();
  puts("LOG: Flushed backlog.");
  last_log.clear();
  for (auto it = log.begin(); it != log.end(); ++it)
    io::Write(fdisk, *it);
  last_log.splice(last_log.begin(), log);
  size = 0;
  fdisk->Close();
}
