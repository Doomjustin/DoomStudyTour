#ifndef DOOM_STUDY_TOUR_FUNCTION_WRAPPER_H
#define DOOM_STUDY_TOUR_FUNCTION_WRAPPER_H

#include <utility>
#include <memory>


namespace DST {

class FunctionWrapper {
public:
template<typename F>
  FunctionWrapper(F&& f)
    : impl_{ new Callable<F>{ std::move(f) } }
  {}

  FunctionWrapper() = default;

  FunctionWrapper(FunctionWrapper&& other) noexcept
    : impl_{ std::move(other.impl_) }
  {}

  FunctionWrapper& operator=(FunctionWrapper&& other) noexcept
  {
    impl_ = std::move(other.impl_);
    return *this;
  }

  FunctionWrapper(FunctionWrapper&) = delete;
  FunctionWrapper(const FunctionWrapper&) = delete;

  void operator()() const { impl_->call(); }

private:
  struct CallableBase {
    ~CallableBase() {}

    virtual void call() = 0;
  };

  template<typename F>
  struct Callable: CallableBase {
    F f_;

    explicit Callable(F&& f)
      : f_{ std::move(f) }
    {}

    ~Callable() {}

    void call() override
    {
      f_();
    }
  };

  std::unique_ptr<CallableBase> impl_;
};


} // namespace DST

#endif // DOOM_STUDY_TOUR_FUNCTION_WRAPPER_H