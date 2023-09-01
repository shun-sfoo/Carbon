#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

class MTVector {
  std::vector<int> m_arr;
  // 读写锁
  mutable std::shared_mutex m_mtx;

public:
  void push_back(int val) {
    // unique_lock RAII 针对 lock()
    std::unique_lock grd(m_mtx);
    m_arr.push_back(val);
  }

  size_t size() const {
    // shared_lock RAII 针对 lock_shared()
    std::shared_lock grd(m_mtx);
    size_t ret = m_arr.size();
    return ret;
  }
};

int main() {
  MTVector arr;

  std::thread t1([&]() {
    for (int i = 0; i < 1000; i++) {
      arr.push_back(i);
    }
  });

  std::thread t2([&]() {
    for (int i = 0; i < 1000; i++) {
      arr.push_back(i);
    }
  });

  t1.join();
  t2.join();

  std::cout << arr.size() << std::endl;
  return 0;
}
