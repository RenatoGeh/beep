#include <cstring>
#include <ctime>
#include <cmath>

#include <string>

#include "chronos.hh"

#define YEAR_SECS 31557600 // Astronomical year -- compromise between common and leap year.
#define MONTH_SECS 2628000 // Month -- average of momnths.
#define WEEK_SECS 604800 // Week.
#define DAY_SECS 86400 // Day.
#define HOUR_SECS 3600 // Hour.
#define MIN_SECS 60 // Minute.
#define SEC_SECS 1 // Second.

namespace {
  const int SECS[] = {YEAR_SECS, MONTH_SECS, DAY_SECS, HOUR_SECS, MIN_SECS, SEC_SECS};
  const char *SECS_S[] = {"years", "months", "days", "hours", "minutes", "seconds"};
  const int N_SECS = 6;
}

namespace chronos {
  Time::Time(void) { time(&raw); }

  Time::Time(std::string stime) { Import(stime); }

  std::string Time::Format(std::string fmt) {
    char buf[256];
    strftime(buf, 256, fmt.c_str(), gmtime(&raw));
    return std::string(buf);
  }

  double Time::operator-(const Time &t) {
    return difftime(raw, t.raw);
  }

  void Time::Import(std::string stime) {
    struct tm stamp;
    strptime(stime.c_str(), "%F %T", &stamp);
    raw = mktime(&stamp);
  }

  std::string Time::Export(void) {
    char buf[32];
    strftime(buf, sizeof(buf), "%F %T", gmtime(&raw));
    return std::string(buf);
  }

  std::string DiffString(double du) {
    int t = (int) fabs(du);
    char rep[512] = "";
    bool neg = du < 0;
    for (int i = 0; i < N_SECS; ++i) {
      int u = t/SECS[i];
      t -= u;
      char buf[64] = "";
      sprintf(buf, "%d %s%s", u, SECS_S[i], (i != N_SECS-1) ? ", " :  (neg ? " ago" : ""));
      strcat(rep, buf);
    }
    return std::string(rep);
  }
}
