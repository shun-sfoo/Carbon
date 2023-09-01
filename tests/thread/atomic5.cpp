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
  std::cout << "equal=" << equal << std::endl;
  std::cout << "old=" << old << std::endl;

  int now = counter.load();
  std::cout << "cnt=" << now << std::endl;

  old = 2;
  equal = counter.compare_exchange_strong(old, 3);
  std::cout << "equal=" << equal << std::endl;
  std::cout << "old=" << old << std::endl;

  now = counter.load();
  std::cout << "cnt=" << now << std::endl;
  return 0;
}
