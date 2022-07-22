#ifndef DOOM_STUDY_TOUR_SINGLETON_H
#define DOOM_STUDY_TOUR_SINGLETON_H

#include <type_traits>


namespace DST {

template<typename T, typename... Args>
using is_singletonable = std::enable_if_t<!std::is_copy_assignable_v<T> &&
      !std::is_copy_constructible_v<T> &&
      std::is_constructible_v<T, Args...>, bool>;


template<typename T, typename... Args>
T* make_singleton(Args&&... args)
{
  static T singleton { std::forward<Args>(args)... };

  return &singleton;
}

} // namespace DST

#endif // DOOM_STUDY_TOUR_SINGLETON_H