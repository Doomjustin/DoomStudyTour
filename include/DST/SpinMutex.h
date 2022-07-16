#ifndef DOOM_STUDY_TOUR_SPIN_MUTEX_H
#define DOOM_STUDY_TOUR_SPIN_MUTEX_H

#include <atomic>


namespace DST {

class SpinMutex {
public:
  SpinMutex()
    : flag_{}
  {}

  void lock()
  {
    while (flag_.test_and_set(std::memory_order_acquire));
  }

  void unlock()
  {
    flag_.clear(std::memory_order_release);
  }

private:
  std::atomic_flag flag_;
};

} // namespace DST

#endif // DOOM_STUDY_TOUR_SPIN_MUTEX_H