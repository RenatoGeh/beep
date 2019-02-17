#ifndef _TIMER_HH
#define _TIMER_HH

#include <thread>
#include <chrono>
#include <vector>
#include <mutex>

#include "updateable.hh"
#include "bot.hh"

#define TIMER_DEF_REFRESH_RATE std::chrono::milliseconds(100)

class Timer {
  public:
    // If default constructor is used, Timer::Initialized must be called!
    Timer(void) : delta(0) {};
    template<class R1, class P1, class R2, class P2> Timer(Bot *bot,
        const std::chrono::duration<R1, P1> &d,
        const std::chrono::duration<R2, P2> &r = TIMER_DEF_REFRESH_RATE);
    ~Timer(void) {}

    template<class R1, class P1, class R2, class P2> void Initialize(Bot *bot,
        const std::chrono::duration<R1, P1> &d,
        const std::chrono::duration<R2, P2> &r = TIMER_DEF_REFRESH_RATE) {
      b = bot;
      delta = std::chrono::duration_cast<decltype(delta)>(d);
      refresh = std::chrono::duration_cast<decltype(refresh)>(r);
      initialized = true;
      running = false;
    }

    void Start(void);
    void Stop(void);

    void Push(Updateable *u);

    bool Running(void) { return running; }

  private:
    bool initialized, running;
    Bot *b;
    std::mutex mu_r, mu_u;
    std::thread t;
    std::chrono::milliseconds delta, refresh;
    std::vector<Updateable*> ups;

    static void f(Bot *b, Timer *self);
};

#undef TIMER_DEF_REFRESH_RATE

#endif
