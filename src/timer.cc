#include <thread>
#include <chrono>
#include <vector>

#include "updateable.hh"

#include "timer.hh"

template<class Rep, class Period> void Timer<Rep, Period>::Start(void) {
  running = true;
  t = std::thread(Timer::f, b, this, delta, ups);
}

template<class Rep, class Period> void Timer<Rep, Period>::Stop(void) {
  running = false;
  t.join();
}

template<class Rep, class Period> void Timer<Rep, Period>::f(Bot *b, Timer<Rep, Period> *self,
    const std::chrono::duration<Rep, Period> &delta, std::vector<Updateable*> ups) {
  while (1) {
    if (!self->running) return;
    std::this_thread::sleep_for(delta);
    for (auto it = ups.begin(); it != ups.end(); ++it)
      (*it)->Update((void*) b);
  }
}

