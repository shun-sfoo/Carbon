#include "logger.h"
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

int main() {
  // condition_variable 必须和unique_lock<std::mutex> 一起用
  std::condition_variable cv;
  std::mutex mtx;
  bool ready = false;

  std::thread t1([&] {
    std::unique_lock lck(mtx);
    // cv.wait(lc, expr)形式,
    // 其中expr是个lambda表达式，只有返回值为true才真正被唤醒，否则继续等待
    cv.wait(lck, [&] { return ready; });
    INFO("t1 is awake");
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(400));

  INFO("notifying not read");
  cv.notify_one(); // useless now, since ready = false

  ready = true;
  INFO("notifying ready");
  cv.notify_one();

  t1.join();
  return 0;
}
