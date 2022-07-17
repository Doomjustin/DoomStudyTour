#include <atomic>
#include <vector>
#include <thread>
#include <cstdlib>

#include <fmt/format.h>


std::atomic<int> count{ 0 };

void add()
{
  for (auto i = 0; i < 10000; ++i)
    count.fetch_add(1, std::memory_order_relaxed);
}


int main()
{
  std::vector<std::thread> vec{};

  for (auto i = 0; i < 10; ++i)
    vec.emplace_back(add);

  for (auto& t: vec)
    t.join();

  fmt::print("result = {}", count);

  return EXIT_SUCCESS;
}