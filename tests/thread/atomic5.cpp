#include "logger.h"
#include <atomic>
#include <ios>
#include <iostream>

int main() {
  std::boolalpha(std::cout);
  std::atomic<int> counter;
  counter.store(2);

  int old = 1;
  // compare_exchange_strong(old,val)
  // 会读取原子变量的值，比较是否和old相等
  // 不相等 则把原子变量的值写入old
  // 相等，则把val写入原子变量
  // old传入的其实是一个引用
  bool equal = counter.compare_exchange_strong(old, 3);
  INFO("equal={}", equal);
  INFO("old={}", old);

  int now = counter.load();
  INFO("cnt={}", now);

  old = 2;
  equal = counter.compare_exchange_strong(old, 3);
  INFO("equal={}", equal);
  INFO("old={}", old);

  now = counter.load();
  INFO("cnt={}", now);
  return 0;
}
