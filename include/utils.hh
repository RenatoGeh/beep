#ifndef _UTILS_HH
#define _UTILS_HH

#include <vector>
#include <string>

#include "libircclient.h"

namespace utils {

  /* Print functions */

  // PrintError prints both the error number and its textual equivalent. Returns the error code.
  int PrintError(irc_session_t *s);

  // PrintError prints an error string. Returns an error code.
  int PrintError(const char *s);

  // PrintError prints an error string, the error number and its textual equivalent. Returns the
  // error code.
  int PrintError(const char *str, irc_session_t *s);

  // PrintfError prints a formatted error string.
  int PrintfError(const char *fmt, ...);

  // PrintError prints a formatted error string, the error number and its textual equivalent.
  // Returns the error code.
  int PrintfError(irc_session_t *s, const char *fmt, ...);

  /* Array functions */

  // Contains returns whether an element has an identical copy of it in the given vector. The type
  // T must be well defined wrt equality (i.e. the operator = must be defined).
  template<typename T> bool Contains(std::vector<T> &a, T e) {
    for (auto i = a.begin(); i != a.end(); ++i)
      if (*i == e) return true;
    return false;
  }

  // Erase erases any occurences of an element from a vector. This is a convenience function for
  // std::remove from <algorithm>.
  template<typename T> void Erase(std::vector<T> &a, T e) {
    a.erase(std::remove(a.begin(), a.end(), e), a.end());
  }

  /* String functions */

  // StripUsername strips a libircclient user string to only the relevant username string.
  std::string StripUsername(std::string user);

  // Sprintf is a C++11 std::string sprintf equivalent.
  std::string Sprintf(const std::string &fmt_str, ...);

  /* Date and time functions */

  // FormatDate returns the current date and time string according to the given format.
  std::string FormatDate(std::string fmt);
}

#endif
