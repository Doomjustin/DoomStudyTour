#ifndef DOOM_STUDY_TOUR_STACK_H
#define DOOM_STUDY_TOUR_STACK_H

#include <atomic>
#include <memory>
#include <exception>
#include <stack>
#include <mutex>


namespace DST {

struct EmptyStack: std::exception {
  const char* what() const
  {
    return "Empty Stack Exception";
  };
};


template<typename T>
class ThreadsafeStack {
public:
  ThreadsafeStack() = default;

  ThreadsafeStack(const ThreadsafeStack& other)
  {
    std::lock_guard<std::mutex> lock{ other.m_ };
    data_ = other.data_;
  }

  ThreadsafeStack& operator=(const ThreadsafeStack& other) = delete;

  void push(T new_value)
  {
    std::lock_guard<std::mutex> lock{ m_ };
    data_.push(std::move(new_value));
  }

  std::shared_ptr<T> pop()
  {
    std::lock_guard<std::mutex> lock{ m_ };
    if (data_.empty())
      throw EmptyStack{};

    const std::shared_ptr<T> res { std::make_shared<T>(std::move(data_.top())) };
    data_.pop();

    return res;
  }

  void pop(T& value)
  {
    std::lock_guard<std::mutex> lock{ m_ };
    if (data_.empty())
      throw EmptyStack{};

    value = std::move(data_.top());
    data_.pop();
  }

  bool empty()
  {
    std::lock_guard<std::mutex> lock{ m_ };
    return data_.empty();
  }

private:
  std::stack<T> data_;
  mutable std::mutex m_;
};

} // namespace DST

#endif // DOOM_STUDY_TOUR_STACK_H