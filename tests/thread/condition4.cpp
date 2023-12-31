#include "Logger.h"
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

// 生产者-消费者模式
int main() {
  // condition_variable 必须和unique_lock<std::mutex> 一起用
  std::condition_variable cv;
  std::mutex mtx;

  std::vector<int> foods;

  std::thread t1([&] {
    for (int i = 0; i < 2; i++) {
      std::unique_lock lck(mtx);
      // wait的过程中会暂时unlock这个锁
      cv.wait(lck, [&] { return foods.size() != 0; });
      auto food = foods.back();
      foods.pop_back();
      lck.unlock();
      INFO("t1 got food:{}", food);
    }
  });

  std::thread t2([&] {
    for (int i = 0; i < 2; i++) {
      std::unique_lock lck(mtx);
      // wait的过程中会暂时unlock这个锁
      cv.wait(lck, [&] { return foods.size() != 0; });
      auto food = foods.back();
      foods.pop_back();
      lck.unlock();
      INFO("t2 got food:{}", food);
    }
  });

  foods.push_back(42);
  cv.notify_one();
  foods.push_back(233);
  cv.notify_one();
  foods.push_back(666);
  foods.push_back(4399);
  cv.notify_all();

  t1.join();
  t2.join();
  return 0;
}
