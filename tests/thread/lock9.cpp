#include "logger.h"
#include <chrono>
#include <mutex>
#include <thread>

// std::unique_lock 具有 mutex所有成员函数
int main() {
  std::mutex mtx;
  std::thread t1([&] {
    std::unique_lock grd(mtx, std::defer_lock);
    std::lock_guard grd2(grd);
    INFO("t1 owns the lock");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  });

  std::thread t2([&] {
    std::unique_lock grd(mtx, std::defer_lock);
    std::lock_guard grd2(grd);
    INFO("t2 owns the lock");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  });

  t1.join();
  t2.join();
  return 0;
}
