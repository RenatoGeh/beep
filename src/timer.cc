#include <cstdio>

#include <thread>
#include <chrono>
#include <vector>

#include "updateable.hh"
#include "utils.hh"

#include "timer.hh"

#define TIMER_REFRESH_RATE 1s

template<class R1, class P1, class R2, class P2> Timer::Timer(Bot *bot,
    const std::chrono::duration<R1, P1> &d,
    const std::chrono::duration<R2, P2> &r) { Initialize(bot, d, r); }

void Timer::Start(void) {
  if (!initialized) {
    utils::PrintError("Attempted to run uninitialized Timer!");
    return;
  } else if (running) return;
  running = true;
  t = std::thread(Timer::f, b, this);
}

void Timer::Stop(void) {
  if (!running) return;
  mu_r.lock();
  running = false;
  mu_r.unlock();
  t.join();
}

void Timer::Push(Updateable *u) {
  mu_u.lock();
  ups.push_back(u);
  mu_u.unlock();
}

void Timer::f(Bot *b, Timer *self) {
  using namespace std::chrono_literals;
  std::chrono::milliseconds elapsed = self->delta;
  while (1) {
    self->mu_r.lock();
    if (!self->running) {
      self->mu_r.unlock();
      return;
    }
    self->mu_r.unlock();

    std::this_thread::sleep_for(self->refresh);
    elapsed -= self->refresh;
    if (elapsed <= 0s) {
      puts("Updating GitLab...");
      self->mu_u.lock();
      for (auto it = self->ups.begin(); it != self->ups.end(); ++it)
        (*it)->Update((void*) b);
      self->mu_u.unlock();
      elapsed = self->delta;
    }
  }
}

