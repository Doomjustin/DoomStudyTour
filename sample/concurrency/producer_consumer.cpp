#include <cstdlib>
#include <atomic>
#include <string>
#include <thread>

#include <fmt/format.h>

std::string work;
std::atomic<bool> ready{ false };

void consumer()
{
  while (!ready.load(std::memory_order_acquire));

  fmt::print("work: {}", work);
}

void producer()
{
  work = "done";
  bool has_ready = false;
  while (ready.compare_exchange_weak(has_ready, true, std::memory_order_release));
}


int main()
{
  std::thread prod{ producer };
  std::thread con{ consumer };
  prod.join();
  con.join();

  return EXIT_SUCCESS;
}