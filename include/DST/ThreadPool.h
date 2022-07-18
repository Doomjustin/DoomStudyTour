#ifndef DOOM_STUDY_TOUR_THREAD_POOL_H
#define DOOM_STUDY_TOUR_THREAD_POOL_H

#include <atomic>
#include <thread>
#include <future>
#include <vector>
#include <functional>
#include <memory>
#include <utility>
#include <type_traits>

#include "Queue.h"
#include "FunctionWrapper.h"


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


class ThreadPool {
private:
  std::atomic<bool> done_;
  ThreadsafeQueue<FunctionWrapper> tasks_;
  std::vector<std::thread> threads_;
  JoinerThreads joiner_;

  void work_thread()
  {
    while (!done_) {
      FunctionWrapper task;
      if (tasks_.try_pop(task))
        task();
      else
        std::this_thread::yield();
    }
  }

public:
  ThreadPool()
    : done_{ false }, joiner_{ threads_ }
  {
    auto size = std::thread::hardware_concurrency() == 0 ?
        4 : std::thread::hardware_concurrency();

    try {
      for (auto i = 0; i < size; ++i)
        threads_.emplace_back(&ThreadPool::work_thread, this);
    }
    catch (...) {
      done_ = true;
      throw;
    }
  }

  template<typename FunctionType>
  auto submit(FunctionType&& f)
    -> std::future<std::invoke_result_t<FunctionType>>
  {
    using result_type = std::invoke_result_t<FunctionType>;

    std::packaged_task<result_type()> task{ std::move(f) };

    auto result = task.get_future();

    tasks_.push(std::move(task));

    return result;
  }

  void run_pending_task()
  {
    FunctionWrapper task;
    if (tasks_.try_pop(task))
      task();
    else
      std::this_thread::yield();
  }
};

} // namespace DST

#endif // DOOM_STUDY_TOUR_THREAD_POOL_H