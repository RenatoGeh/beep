#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <ctime>

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include "boost/tokenizer.hpp"

#include "libircclient.h"

#include "utils.hh"

namespace utils {

  /* Print functions */

  int PrintError(irc_session_t *s) {
    int e = irc_errno(s);
    printf("Error %d: %s\n", e, irc_strerror(e));
    return e;
  }

  int PrintError(const char *s) {
    printf("Error: %s\n", s);
    return 1;
  }

  int PrintError(const char *str, irc_session_t *s) {
    int e = irc_errno(s);
    printf("Error %d: %s\n  > %s\n", e, str, irc_strerror(e));
    return e;
  }

  int PrintfError(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    return 1;
  }

  int PrintfError(irc_session_t *s, const char *fmt, ...) {
    int e = irc_errno(s);
    printf("Error %d: ", e);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n  > %s\n", irc_strerror(e));
    return e;
  }

  /* Array functions */

  /* String functions */

  std::string StripUsername(std::string user) {
    return user.substr(0, user.find("!"));
  }

  std::vector<std::string> Tokenize(std::string s) {
    boost::char_separator<char> sep(" ");
    boost::tokenizer<boost::char_separator<char>> t(s, sep);
    std::vector<std::string> l;
    for (auto it = t.begin(); it != t.end(); ++it)
      l.push_back(*it);
    return l;
  }
}
