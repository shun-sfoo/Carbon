#include <cstdio>
#include <mutex>

std::mutex mtx;
int main() {
  if (mtx.try_lock()) {
    printf("succeed\n");
  } else {
    printf("failed\n");
  }

  if (mtx.try_lock()) {
    printf("succeed\n");
  } else {
    printf("failed\n");
  }

  mtx.unlock();
  return 0;
}
