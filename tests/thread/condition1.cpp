#include "logger.h"
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

int main() {
  // condition_variable 必须和unique_lock<std::mutex> 一起用
  std::condition_variable cv;
  std::mutex mtx;

  std::thread t1([&] {
    std::unique_lock lck(mtx);
    // wait会让当前线程陷入等待
    cv.wait(lck);
    INFO("t1 is awake");
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(400));

  INFO("notifying...");
  // 会唤醒陷入等待的线程
  cv.notify_one(); // will awake t1

  t1.join();
  return 0;
}
