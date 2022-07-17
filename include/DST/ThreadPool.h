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


namespace DST {


namespace v1 {

class JoinThreads {
public:
  explicit JoinThreads(std::vector<std::thread>& threads)
    : threads_{ threads }
  {}

  ~JoinThreads()
  {
    for (auto i = 0; i < threads_.size(); ++i)
      if (threads_[i].joinable())
        threads_[i].join();
  }

private:
  std::vector<std::thread>& threads_;
};


class ThreadPool {
private:
  std::atomic<bool> done_;
  ThreadsafeQueue<std::function<void()>> work_queue_;
  std::vector<std::thread> threads_;
  JoinThreads joiner;

  void worker_thread()
  {
    while (!done_) {
      std::function<void()> task;
      if (work_queue_.try_pop(task))
        task();
      else
        std::this_thread::yield();
    }
  }

public:
  ThreadPool()
    : done_{ false }, joiner{ threads_ }
  {
    auto thread_count = std::thread::hardware_concurrency();

    try {
      for (auto i = 0; i < thread_count; ++i)
        threads_.push_back(std::thread(&ThreadPool::worker_thread, this));
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

  template<typename FunctionType>
  void submit(FunctionType f)
  {
    work_queue_.push(std::function<void()>{ f });
  }

};

} // namespace v1

namespace v2 {

// 感觉比函数指针的用法更麻烦点
class FunctionWrapper {
private:
  struct ImplBase {
    ~ImplBase() {}

    virtual void call() = 0;
  };

  template<typename F>
  struct ImplType: ImplBase {
    F f;
    ImplType(F&& func)
      : f{ std::move(func) }
    {}

    void call() override { f(); }
  };

  std::unique_ptr<ImplBase> impl_;

public:
  template<typename F>
  FunctionWrapper(F&& f)
    : impl_{ new ImplType<F>(std::move(f)) }
  {}

  FunctionWrapper() = default;

  FunctionWrapper(FunctionWrapper&& other)
    : impl_{ std::move(other.impl_) }
  {}

  FunctionWrapper& operator=(FunctionWrapper&& other)
  {
    impl_ = std::move(other.impl_);
    return *this;
  }

  FunctionWrapper(FunctionWrapper&) = delete;
  FunctionWrapper(const FunctionWrapper&) = delete;
  FunctionWrapper& operator=(const FunctionWrapper&) = delete;

  void operator()() { impl_->call(); }
};


class ThreadPool {
private:
  std::atomic<bool> done_;
  ThreadsafeQueue<FunctionWrapper> work_queue_;

  void worker_thread()
  {
    while (!done_) {
      FunctionWrapper task;
      if (work_queue_.try_pop(task))
        task();
      else
        std::this_thread::yield();
    }
  }

public:
  template<typename FunctionType>
  std::future<std::invoke_result_t<FunctionType()>>
    submit(FunctionType f)
  {
    using result_type = std::invoke_result_t<FunctionType()>;

    std::packaged_task<result_type()> task{ std::move(f) };
    std::future<result_type> res{ task.get_future() };
    work_queue_.push(std::move(task));

    return res;
  }
};

} // namespace v2

} // namespace DST

#endif // DOOM_STUDY_TOUR_THREAD_POOL_H