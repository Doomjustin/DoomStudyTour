#ifndef DOOM_STUDY_TOUR_FUNCTION_WRAPPER_H
#define DOOM_STUDY_TOUR_FUNCTION_WRAPPER_H

#include <utility>
#include <memory>


namespace DST {

class FunctionWrapper {
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

private:
  struct ImplBase {
    ~ImplBase() {}

    virtual void call() = 0;
  };

  template<typename F>
  struct ImplType: ImplBase {
    F invoker_;

    ImplType(F&& f)
      : invoker_{ std::forward<F>(f) }
    {}

    void call() override
    {
      invoker_();
    }
  };

  std::unique_ptr<ImplBase> impl_;
};


} // namespace DST

#endif // DOOM_STUDY_TOUR_FUNCTION_WRAPPER_H