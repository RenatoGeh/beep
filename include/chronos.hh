#ifndef _CHRONOS_HH
#define _CHRONOS_HH

#include <ctime>

#include <string>

namespace chronos {
  // Time is time.
  class Time {
    public:
      // Creates a new Time using the current GMT time.
      Time(void);
      // Creates a new Time importing a timestamp from a string.
      Time(std::string stime);
      // Format returns a string according to the strftime format.
      std::string Format(std::string fmt, bool utc = false);
      // The operator- overload measures the duration between this Time and the given Time.
      double operator-(const Time &t);
      // Import imports a timestamp in string format. Requires POSIX.
      void Import(std::string stime);
      // Export exports this Time to string format.
      std::string Export(void);
      // Reset sets this Time to the current time.
      void Reset(void) { time(&raw); }
    private:
      time_t raw;
  };

  // DiffString returns a string representation of a duration.
  std::string DiffString(double d);

  // DiffString returns a string representation of a duration t-s.
  inline std::string DiffString(const Time &s, Time &t) { return DiffString(t-s); }

  // Now returns the time right now in string format.
  std::string Now(void);
}

#endif
