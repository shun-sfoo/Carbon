#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

int main() {
  // condition_variable 必须和unique_lock<std::mutex> 一起用
  std::condition_variable cv;
  std::mutex mtx;

  std::thread t1([&] {
    std::unique_lock lck(mtx);
    // wait的过程中会暂时unlock这个锁
    cv.wait(lck);
    std::cout << "t1 is awake" << std::endl;
  });

  std::thread t2([&] {
    std::unique_lock lck(mtx);
    cv.wait(lck);
    std::cout << "t2 is awake" << std::endl;
  });

  std::thread t3([&] {
    std::unique_lock lck(mtx);
    cv.wait(lck);
    std::cout << "t3 is awake" << std::endl;
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(400));
  std::cout << "notifying neo" << std::endl;
  cv.notify_one(); // useless now, since ready = false

  std::this_thread::sleep_for(std::chrono::milliseconds(400));
  std::cout << "notifying all" << std::endl;
  cv.notify_all();

  t1.join();
  t2.join();
  t3.join();

  return 0;
}
