#ifndef _INSTANT_LOG_HH
#define _INSTANT_LOG_HH

#include "io.hh"

#include <string>
#include <list>

#ifndef _INSTANT_LOG_LOG_CACHE
#define _INSTANT_LOG_LOG_CACHE 200
#endif

// InstantLog logs a text message instantly (once it receives the message, it immediately writes it
// on disk.
class InstantLog {
  public:
    InstantLog(std::string filename, int cache_size = _INSTANT_LOG_LOG_CACHE);
    InstantLog(void) = delete;

    virtual ~InstantLog(void);

    // Log logs a message text from user to the InstantLog.
    virtual unsigned int Log(std::string user, const std::string &text);

    // Flush flushes the contents of log into fdisk.
    virtual void Flush(void) {}

    const std::list<std::string> Cache(void) { return cache; }

  protected:
    io::File *fdisk;
    std::string fname;
    std::list<std::string> cache;
};

#endif
