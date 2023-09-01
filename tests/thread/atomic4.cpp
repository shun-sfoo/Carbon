#include <atomic>
#include <iostream>

int main() {
  std::atomic<int> counter;
  counter.store(0);

  // exchange 会把val写入原子变量，同时返回其旧的值
  int old = counter.exchange(3);
  std::cout << "old=" << old << std::endl;

  int now = counter.load();
  std::cout << "cnt=" << now << std::endl;
  return 0;
}
