
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

// 这种写法，download 的执行仍在主线程中，
// 他只是函数式编程范式意义上的异步，而不涉及到真正的多线程。
// 可以用这个实现惰性求值（lazy evaluation）之类。
int main() {
  // std::async 的第一个参数可以设为 std::launch::deferred,
  // 这时不会创建一个线程来执行
  std::future<int> fret =
      std::async(std::launch::deferred, [&] { return download("hello.zip"); });
  interact();
  // std::shared_future<void> fret
  // auto fret2 = fret; 浅拷贝，共享同一个future对象

  // lambda 函数体内的运算推迟到 future 的 get() 被调用时。
  int ret = fret.get();
  std::cout << "Download result: " << ret << std::endl;
  return 0;
}
