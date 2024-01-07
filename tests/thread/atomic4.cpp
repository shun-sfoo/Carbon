#include "Logger.h"
#include <atomic>

int main() {
  std::atomic<int> counter;
  counter.store(0);

  // exchange 会把val写入原子变量，同时返回其旧的值
  int old = counter.exchange(3);
  INFO("old={}", old);

  int now = counter.load();
  INFO("cnt={}", now);
  return 0;
}
