#include "Logger.h"
#include <mutex>
#include <thread>
#include <vector>
int main() {
  std::vector<int> arr;
  std::mutex mtx;
  std::thread t1([&] {
    for (int i = 0; i < 1000; i++) {
      std::unique_lock grd(mtx);
      arr.push_back(1);
    }
  });
  std::thread t2([&] {
    for (int i = 0; i < 1000; i++) {
      // 指定了 std::defer_lock 不会在构造器中调用mtx.lock()
      // 需要之后再手动调用grd.lock才能上锁
      std::unique_lock grd(mtx, std::defer_lock);
      INFO("before the lock");
      grd.lock();
      arr.push_back(2);
      // 好处依然是即使忘记grd.unlock()也能够自动调用mtx.unlock()
      grd.unlock();
      INFO("outside of lock");
    }
  });
  t1.join();
  t2.join();
  return 0;
}
