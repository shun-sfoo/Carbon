#include <atomic>
#include <iostream>
#include <thread>

int main() {
  std::atomic<int> counter = 0;

  std::thread t1([&] {
    for (int i = 0; i < 10000; i++) {
      // 等价于 +=
      counter.fetch_add(1);
    }
  });

  std::thread t2([&] {
    for (int i = 0; i < 10000; i++) {
      counter.fetch_add(1);
    }
  });

  t1.join();
  t2.join();

  // store 对应于 =
  // 用于 读取
  std::cout << counter.load() << std::endl;
  return 0;
}
