#ifndef _BACKLOG_HH
#define _BACKLOG_HH

#include <list>
#include <string>

#include "io.hh"

class Backlog {
  public:
    Backlog(std::string filename, unsigned int c);
    Backlog(void) = delete;

    ~Backlog(void);

    // Log logs a message text from user to the Backlog. This does not guarantee write on disk.
    // Write on disk (i.e. Flush) is only triggered either through a manual function call or when
    // this->size > this->cap.
    unsigned int Log(std::string user, const std::string &text);

    // Flush flushes
    void Flush(void);
  private:
    io::File *fdisk;
    std::list<std::string> log;
    std::list<std::string> last_log;
    unsigned int size;
    unsigned int cap;
};

#endif
