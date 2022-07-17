#include <atomic>
#include <vector>
#include <thread>
#include <cstdlib>

#include <fmt/format.h>


std::vector<int> my_shared_work;
std::atomic<bool> data_produced{ false };
std::atomic<bool> data_consumed{ false };

void data_producer()
{
  my_shared_work = { 1, 0, 3 };
  data_produced.store(true, std::memory_order_release);
}

void delivery_boy()
{
  while (!data_produced.load(std::memory_order_acquire));
  data_consumed.store(true, std::memory_order_release);
}

void data_consumer()
{
  while (!data_consumed.load(std::memory_order_acquire));
  my_shared_work[1] = 2;
}


int main()
{
  std::thread t1{ data_consumer };
  std::thread t2{ delivery_boy };
  std::thread t3{ data_producer };

  t1.join();
  t2.join();
  t3.join();

  fmt::print("my shared work: {}, {}, {}", my_shared_work[0], my_shared_work[1], my_shared_work[2]);

  return EXIT_SUCCESS;
}