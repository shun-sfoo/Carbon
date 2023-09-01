#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <thread>

int download(std::string file) {
  for (int i = 0; i < 10; i++) {
    std::cout << "Downloading " << file << " (" << i * 10 << "%)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
  }
  std::cout << "Download complete: " << file << std::endl;
  return 404;
}

void interact() {
  std::string name;
  std::cin >> name;
  std::cout << "Hi, " << name << std::endl;
}

// 如果不想让 std::async 帮你自动创建线程，
// 想要手动创建线程，可以直接用 std::promise
int main() {
  std::promise<int> pret;
  std::thread t1([&] {
    auto ret = download("hello.zip");
    pret.set_value(ret); // 在线程返回的时候，用 set_value() 设置返回值
  });

  // 用 get_future() 获取其 std::future 对象
  std::future<int> fret = pret.get_future();

  interact();
  int ret = fret.get(); // get() 可以等待并获取线程返回值
  std::cout << "Download result: " << ret << std::endl;
  t1.join();
  return 0;
}
