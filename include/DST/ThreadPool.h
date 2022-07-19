#ifndef DOOM_STUDY_TOUR_THREAD_POOL_H
#define DOOM_STUDY_TOUR_THREAD_POOL_H

#include <atomic>
#include <thread>
#include <future>
#include <vector>
#include <queue>
#include <memory>
#include <utility>
#include <functional>
#include <condition_variable>
#include <mutex>

#include <fmt/format.h>

#include "JoinerThreads.h"
#include "FunctionWrapper.h"


namespace DST {

class ThreadPool {
public:
  using size_type = decltype(std::thread::hardware_concurrency());

  ThreadPool()
    : ThreadPool{ std::thread::hardware_concurrency() }
  {}

  ThreadPool(size_type size)
    : done_{ false },
      threads_(size),
      joiner_{ threads_ }
  {
    try {
      for (auto i = 0; i < threads_.size(); ++i)
        threads_.emplace_back(&ThreadPool::schedule, this);
    }
    catch (...) {
      done_ = true;
      throw;
    }
  }

  template<typename F>
  std::future<std::invoke_result_t<F>>
    submit(F f)
  {
    using result_type = std::invoke_result_t<F>;
    std::packaged_task<result_type()> task { std::move(f) };
    auto res = task.get_future();
    {
      std::lock_guard<std::mutex> lock{ m_ };
      tasks_.push(std::move(task));
    }
    cv_.notify_one();

    return res;
  }

private:
  std::atomic<bool> done_;
  std::mutex m_;
  std::condition_variable cv_;
  std::queue<FunctionWrapper> tasks_;
  std::vector<std::thread> threads_;
  JoinerThreads joiner_;

  FunctionWrapper get_pending_task()
  {
    std::unique_lock<std::mutex> lock{ m_ };
    cv_.wait(lock, [this] { return !tasks_.empty(); });

    auto task = std::move(tasks_.front());
    tasks_.pop();

    return task;
  }

  void schedule()
  {
    auto task = get_pending_task();
    task();
  }
};

} // namespace DST

#endif // DOOM_STUDY_TOUR_THREAD_POOL_H