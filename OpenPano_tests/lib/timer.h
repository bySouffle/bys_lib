//
// Created by bys on 2022/6/27.
//

#ifndef BYS_LIB_OPENPANO_TESTS_LIB_TIMER_H_
#define BYS_LIB_OPENPANO_TESTS_LIB_TIMER_H_


#include <chrono>
#include <functional>
#include <map>
#include <iostream>
#include <string>
#include <utility>

class Timer {
 public:
  using Clock = std::chrono::high_resolution_clock;
  Timer() {
    restart();
  }

  // return current unix timestamp
  void restart() {
    m_start_time = std::chrono::high_resolution_clock::now();
  }

  // return duration in seconds
  double duration() const {
    auto now = std::chrono::high_resolution_clock::now();
    auto m = std::chrono::duration_cast<std::chrono::microseconds>(now - m_start_time).count();
    return m * 1.0 / 1e6;
  }

 protected:
  std::chrono::time_point<Clock> m_start_time;

};


class GuardedTimer: public Timer {
 public:
  GuardedTimer(const std::string& msg,  bool enabled=true):
      GuardedTimer([msg](double duration){
        std::cout << msg << ": " << std::to_string(duration * 1000.) << " milliseconds." << std::endl;
      })
  { enabled_ = enabled; }

  GuardedTimer(const char* msg, bool enabled=true):
      GuardedTimer(std::string(msg), enabled) {}

  GuardedTimer(std::function<void(double)> callback):
      m_callback(std::move(callback))
  { }

  ~GuardedTimer() {
    if (enabled_)
      m_callback(duration());
  }

 protected:
  bool enabled_;
  std::function<void(double)> m_callback;

};

// record the total running time of a region across the lifecycle of the whole program
// call TotalTimer::print() before exiting main()
class TotalTimer {
 public:
  TotalTimer(const std::string& msg):
      msg(msg) {
    timer.restart();
  }

  ~TotalTimer();

  static void print();

  std::string msg;
  Timer timer;

  static std::map<std::string, std::pair<int, double>> rst;
};

// Build a global instance of this class, to call print() before program exit.
struct TotalTimerGlobalGuard {
  ~TotalTimerGlobalGuard() { TotalTimer::print(); }
};


#define GUARDED_FUNC_TIMER \
	GuardedTimer _long_long_name_guarded_timer(__func__)

#define TOTAL_FUNC_TIMER \
	TotalTimer _long_long_name_total_timer(__func__)



#endif //BYS_LIB_OPENPANO_TESTS_LIB_TIMER_H_
