#ifndef _TIMER_HH
#define _TIMER_HH

#include <thread>
#include <chrono>
#include <vector>

#include "updateable.hh"
#include "bot.hh"

template<class Rep, class Period> class Timer {
  public:
    Timer(Bot *bot, const std::chrono::duration<Rep, Period> &d) : b(bot), delta(d) {}
    ~Timer(void) {}

    void Start(void);
    void Stop(void);

    bool Running(void) { return running; }

  private:
    bool running;
    Bot *b;
    std::thread t;
    const std::chrono::duration<Rep, Period> delta;
    std::vector<Updateable*> ups;

    static void f(Bot *b, Timer<Rep, Period> *self, const std::chrono::duration<Rep, Period> &delta,
        std::vector<Updateable*> ups);
};

#endif
