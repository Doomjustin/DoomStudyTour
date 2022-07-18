#ifndef DOOM_STUDY_TOUR_JOINER_THREADS_H
#define DOOM_STUDY_TOUR_JOINER_THREADS_H

#include <vector>
#include <thread>


namespace DST {

class JoinerThreads {
private:
  std::vector<std::thread>& threads_;

public:
  explicit JoinerThreads(std::vector<std::thread>& threads)
    : threads_{ threads }
  {}

  ~JoinerThreads()
  {
    for (auto& t: threads_)
      if (t.joinable())
        t.join();
  }
};


} // namespace DST

#endif // DOOM_STUDY_TOUR_JOINER_THREADS_H