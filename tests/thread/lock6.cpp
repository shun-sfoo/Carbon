#include "logger.h"
#include <chrono>
#include <mutex>

std::timed_mutex mtx;

int main() {
  // 等待一段时间
  // try_lock_until()
  if (mtx.try_lock_for(std::chrono::milliseconds(500))) {
    INFO("succeed");
  } else {
    INFO("failed");
  }

  if (mtx.try_lock_for(std::chrono::milliseconds(500))) {
    INFO("succeed");
  } else {
    INFO("failed");
  }

  mtx.unlock();
  return 0;
}
