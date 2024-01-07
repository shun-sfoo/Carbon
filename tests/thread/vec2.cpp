#include "Logger.h"
#include <shared_mutex>
#include <thread>
#include <vector>

class MTVector {
  std::vector<int> m_arr;
  // 读写锁
  mutable std::shared_mutex m_mtx;

public:
  void push_back(int val) {
    m_mtx.lock();
    m_arr.push_back(val);
    m_mtx.unlock();
  }

  size_t size() const {
    m_mtx.lock_shared();
    size_t ret = m_arr.size();
    m_mtx.unlock_shared();
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

  INFO("{}", arr.size());
  return 0;
}
