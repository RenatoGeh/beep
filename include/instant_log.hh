#ifndef _INSTANT_LOG_HH
#define _INSTANT_LOG_HH

#include "io.hh"

#include <string>

// InstantLog logs a text message instantly (once it receives the message, it immediately writes it
// on disk.
class InstantLog {
  public:
    InstantLog(std::string filename);
    InstantLog(void) = delete;

    virtual ~InstantLog(void);

    // Log logs a message text from user to the InstantLog.
    virtual unsigned int Log(std::string user, const std::string &text);

    // Flush flushes the contents of _log into fdisk.
    virtual void Flush(void);

  protected:
    io::File *fdisk;
    std::string fname;

  private:
    std::string _log;
};

#endif
