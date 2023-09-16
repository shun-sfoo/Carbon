#include <atomic>
#include <thread>
#include "logger.h"

int main() {
  std::atomic<int> counter = 0;

  std::thread t1([&] {
    for (int i = 0; i < 10000; i++) {
      // counter = counter + 1; 错
      //  counter += 1; 对
      //  counter++; 对
      counter++;
    }
  });

  std::thread t2([&] {
    for (int i = 0; i < 10000; i++) {
      counter += 1;
    }
  });

  t1.join();
  t2.join();

  INFO("{}", counter);
  return 0;
}
