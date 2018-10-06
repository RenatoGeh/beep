#ifndef _DATE_HH
#define _DATE_HH

#include <ctime>

#include <string>

namespace chronos {
  // Time is time.
  class Time {
    public:
      Time(void);
      // Format returns a string according to the strftime format.
      std::string Format(std::string fmt);
      // The operator- overload measures the duration between this Time and the given Time.
      double operator-(const Time &t);
    private:
      time_t raw;
  };

  // DiffString returns a string representation of a duration.
  std::string DiffString(double d);

  // DiffString returns a string representation of a duration t-s.
  std::string DiffString(const Time &s, Time &t) { return DiffString(t-s); }
}

#endif
