#include <mutex>
#include <thread>
#include <vector>

int main() {
  std::vector<int> arr;
  std::mutex mtx;
  std::thread t1([&] {
    for (int i = 0; i < 1000; i++) {
      // unique_lock 也符合RAII，自由度更高
      std::unique_lock grd(mtx);
      arr.push_back(1);
    }
  });

  std::thread t2([&] {
    for (int i = 0; i < 1000; i++) {
      std::unique_lock grd(mtx);
      arr.push_back(2);
      grd.unlock();
      printf("outside of lock\n");
      // grd.lock();
    }
  });
  t1.join();
  t2.join();
  return 0;
}
