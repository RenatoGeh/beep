#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <ctime>

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

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

  std::string Sprintf(const std::string &fmt_str, ...) {
    int final_n, n = ((int)fmt_str.size()) * 2;
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while(1) {
      formatted.reset(new char[n]);
      strcpy(&formatted[0], fmt_str.c_str());
      va_start(ap, fmt_str);
      final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
      va_end(ap);
      if (final_n < 0 || final_n >= n)
        n += abs(final_n - n + 1);
      else
        break;
    }
    return std::string(formatted.get());
  }
}
