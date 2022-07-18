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

#include "FunctionWrapper.h"
#include "JoinerThreads.h"


namespace DST {

class ThreadPool {
private:
  using task_type = std::function<void()>;
  std::atomic<bool> done_;
  std::queue<task_type> tasks_;
  std::vector<std::thread> threads_;
  JoinerThreads joiner_;
  std::mutex mutex_;
  std::condition_variable cv_;

  task_type get_pending_task()
  {
    std::unique_lock<std::mutex> lock{ mutex_ };
    cv_.wait(lock, [this]{ return !tasks_.empty(); });
    auto task = std::move(tasks_.front());
    tasks_.pop();

    return std::move(task);
  }

  void work_thread()
  {
    while (!done_) {
      auto pending_task = std::move(get_pending_task());
      if (pending_task)
        pending_task();
    }
  }

public:
  ThreadPool()
    : ThreadPool{ std::thread::hardware_concurrency() }
  {}

  explicit ThreadPool(std::size_t size)
    : done_{ false }, threads_(size), joiner_{ threads_ }
  {
    try {
      for (auto i = 0; i < size; ++i)
        threads_.emplace_back(&ThreadPool::work_thread, this);
    }
    catch (...) {
      done_ = true;
      throw;
    }
  }

  ~ThreadPool()
  {
    done_ = true;
  }

  template<typename F, typename... Args>
  auto submit(F&& f, Args&&... args)
    -> std::future<decltype(f(args...))>
  {
    using result_type = decltype(f(args...));

    auto task = std::make_shared<std::packaged_task<result_type()>>(
          std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<result_type> result = task->get_future();
    {
      std::lock_guard<std::mutex> lock{ mutex_ };
      tasks_.emplace([task] { (*task)(); });
    }
    cv_.notify_all();

    return result;
  }
};

} // namespace DST

#endif // DOOM_STUDY_TOUR_THREAD_POOL_H