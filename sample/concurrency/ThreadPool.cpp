#include <cstdlib>
#include <queue>

#include <fmt/format.h>

#include "DST/ThreadPool.h"


int main()
{
  DST::ThreadPool pool {};

  auto res = pool.submit([] { return 10; });

  fmt::print("result = {}\n", res.get());

  return EXIT_SUCCESS;
}