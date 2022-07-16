#ifndef DOOM_STUDY_TOUR_QUEUE_H
#define DOOM_STUDY_TOUR_QUEUE_H

#include <memory>
#include <mutex>
#include <condition_variable>
#include <utility>


namespace DST {

template<typename T>
class ThreadsafeQueue {
private:
  struct Node {
    std::shared_ptr<T> data;
    std::unique_ptr<Node> next;
  };

  std::mutex head_mutex_;
  std::unique_ptr<Node> head_;
  std::mutex tail_mutex_;
  Node* tail_;
  std::condition_variable condition_;

  Node* get_tail()
  {
    std::lock_guard<std::mutex> tail_lock{ tail_mutex_ };
    return tail_;
  }

  std::unique_ptr<Node> pop_head()
  {
    auto old_head = std::move(head_);
    head_ = std::move(old_head->next);

    return old_head;
  }

  std::unique_lock<std::mutex> wait_for_data()
  {
    std::unique_lock<std::mutex> head_lock{ head_mutex_ };
    condition_.wait(head_lock, [&] { return head_.get() != get_tail(); });

    return std::move(head_lock);
  }

  std::unique_ptr<Node> wait_pop_head()
  {
    std::unique_lock<std::mutex> head_lock{ wait_for_data() };
    return pop_head();
  }

  std::unique_ptr<Node> wait_pop_head(T& value)
  {
    std::unique_lock<std::mutex> haed_lock{ wait_for_data() };
    value = std::move(*head_->data);
    return pop_head();
  }

  std::unique_ptr<Node> try_pop_head()
  {
    std::lock_guard<std::mutex> head_lock{ head_mutex_ };
    if (head_.get() == get_tail())
      return std::unique_ptr<Node>{};

    return pop_head();
  }

  std::unique_ptr<Node> try_pop_head(T& value)
  {
    std::lock_guard<std::mutex> head_lock{ head_mutex_ };
    if (head_.get() == get_tail())
      return std::unique_ptr<Node>{};

    return pop_head();
  }

public:
  ThreadsafeQueue()
    : head_{ new Node{} }, tail_{ head_.get() }
  {}

  ThreadsafeQueue(const ThreadsafeQueue&) = delete;
  ThreadsafeQueue& operator=(const ThreadsafeQueue&) = delete;

  std::shared_ptr<T> try_pop()
  {
    std::unique_ptr<Node> old_head = try_pop_head();
    return old_head ? old_head->data : std::shared_ptr<T>{};
  }

  bool try_pop(T& value)
  {
    std::unique_ptr<Node> const old_head = try_pop_head(value);
    return old_head;
  }

  std::shared_ptr<T> wait_and_pop()
  {
    const std::unique_ptr<Node> old_head = wait_pop_head();
    return old_head->data;
  }

  void wait_and_pop(T& value)
  {
    std::unique_ptr<Node> const old_head = wait_pop_head(value);
  }

  void push(T new_value)
  {
    std::shared_ptr<T> new_data{ std::make_shared<T>(std::move(new_value)) };
    std::unique_ptr<Node> p{ new Node{} };
    {
      std::lock_guard<std::mutex> tail_lock{ tail_mutex_ };
      tail_->data = new_data;
      Node* const new_tail = p.get();
      tail_->next = std::move(p);
      tail_ = new_tail;
      condition_.notify_one();
    }
  }

  bool empty()
  {
    std::lock_guard<std::mutex> head_lock{ head_mutex_ };
    return head_.get() == get_tail();
  }
};

template<typename T>
void ThreadsafeQueue<T>::push(T new_value)
{
  std::shared_ptr<T> new_data{ std::make_shared<T>(std::move(new_value)) };
  std::unique_ptr<Node> p{ new Node{} };
  {
    std::lock_guard<std::mutex> tail_lock{ tail_mutex_ };
    tail_->data = new_data;
    Node* const new_tail = p.get();
    tail_->next = std::move(p);
    tail_ = new_tail;
    condition_.notify_one();
  }
}

} // namespace DST

#endif // DOOM_STUDY_TOUR_QUEUE_H