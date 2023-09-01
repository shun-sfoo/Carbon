#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <thread>

void download(std::string file) {
  for (int i = 0; i < 10; i++) {
    std::cout << "Downloading " << file << " (" << i * 10 << "%)..."
              << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
  }
  std::cout << "Download complete: " << file << std::endl;
}

void interact() {
  std::string name;
  std::cin >> name;
  std::cout << "Hi, " << name << std::endl;
}

int main() {
  // std::async 返回类型可以为void
  // std::promise<void> 他的set_value不接受任何参数，仅仅作为同步用
  std::shared_future<void> fret = std::async([&] { download("hello.zip"); });
  // 共享同一个future对象
  auto fret2 = fret;
  auto fret3 = fret;
  interact();
  fret3.wait();
  std::cout << "Download complete" << std::endl;
  return 0;
}
