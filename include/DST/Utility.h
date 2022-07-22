#ifndef DOOM_STUDY_TOUR_UTILITY_H
#define DOOM_STUDY_TOUR_UTILITY_H

#include <type_traits>


namespace DST {

/**
 * @brief C风格指针强制转换
 *    int* a;
 *    long* b;
 *    b = prt_cast<long*>(a);
 * @tparam To
 * @tparam From
 * @param to
 * @return From
 */
template<typename To, typename From,
      std::enable_if_t<std::is_pointer_v<To> &&
        std::is_pointer_v<From>, bool> = true>
To ptr_cast(From from)
{
  return (To) from;
}

} // namespace DST

#endif // DOOM_STUDY_TOUR_UTILITY_H