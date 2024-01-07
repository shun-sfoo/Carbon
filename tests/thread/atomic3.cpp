#include "Logger.h"
#include <atomic>
#include <thread>
#include <vector>

int main() {
  std::atomic<int> counter;
  counter.store(0);

  std::vector<int> data(20000);

  std::thread t1([&] {
    for (int i = 0; i < 10000; i++) {
      // fetch_add会返回旧值
      int index = counter.fetch_add(1);
      data[index] = i;
    }
  });

  std::thread t2([&] {
    for (int i = 0; i < 10000; i++) {
      int index = counter.fetch_add(1);
      data[index] = i + 10000;
    }
  });

  t1.join();
  t2.join();

  INFO("{}", data[10000]);
  return 0;
}
