#include <chrono>
#include <mutex>

std::timed_mutex mtx;

int main() {
  // 等待一段时间
  // try_lock_until()
  if (mtx.try_lock_for(std::chrono::milliseconds(500))) {
    printf("succeed\n");
  } else {
    printf("failed\n");
  }

  if (mtx.try_lock_for(std::chrono::milliseconds(500))) {
    printf("succeed\n");
  } else {
    printf("failed\n");
  }

  mtx.unlock();
  return 0;
}
