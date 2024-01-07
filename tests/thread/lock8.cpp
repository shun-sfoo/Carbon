#include "Logger.h"
#include <chrono>
#include <mutex>
#include <thread>

int main() {
  std::mutex mtx;
  std::thread t1([&] {
    std::unique_lock grd(mtx);
    INFO("t1 owns the lock");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  });

  // 当前mutex 已经上锁，但是之后仍然希望用RAII思想在解构的时候调用unlock()
  // 可以用adopt_lock作为unique_lock或lock_guard第二参数
  // 这时他们会默认mtx已经上锁
  std::thread t2([&] {
    mtx.lock();
    std::unique_lock grd(mtx, std::adopt_lock);
    INFO("t2 owns the lock");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  });

  t1.join();
  t2.join();
  return 0;
}
