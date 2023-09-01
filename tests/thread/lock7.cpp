
#include <chrono>
#include <mutex>
#include <thread>
int main() {
  std::mutex mtx;
  std::thread t1([&] {
    // 与无参数相比 他会调用mtx.try_lock() 而不是mtx.lock()
    // 之后用grd.owns_lock() 判断是否上锁成功
    std::unique_lock grd(mtx, std::try_to_lock);
    if (grd.owns_lock()) {
      printf("t1 success\n");
    } else {
      printf("t1 failed\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  });

  std::thread t2([&] {
    std::unique_lock grd(mtx, std::try_to_lock);
    if (grd.owns_lock()) {
      printf("t2 success\n");
    } else {
      printf("t2 failed\n");
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  });
  t1.join();
  t2.join();
  return 0;
}
