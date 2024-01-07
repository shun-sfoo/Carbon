#include "Logger.h"
#include <mutex>

std::mutex mtx;
int main() {
  if (mtx.try_lock()) {
    INFO("succeed");
  } else {
    INFO("failed");
  }

  if (mtx.try_lock()) {
    INFO("succeed");
  } else {
    INFO("failed");
  }

  mtx.unlock();
  return 0;
}
