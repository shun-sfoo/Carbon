#include "Logger.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
using namespace carbon;

class ThreadPool {
private:
  std::vector<std::thread> m_pool;

public:
  void push_back(std::thread thr) { m_pool.push_back(std::move(thr)); }

  ~ThreadPool() {
    for (auto &t : m_pool) {
      t.join();
    }
  }
};

ThreadPool tpool;

void download(std::string file) {
  for (int i = 0; i < 10; i++) {
    INFO("Downloading {} ({}%)", file, i * 10);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
  }
  INFO("Download complete: {}", file);
}

void interact() {
  std::string name;
  std::cin >> name;
  INFO("Hi, {}", name);
}

void myfunc() {
  std::thread t1([&] { download("hello.zip"); });
  tpool.push_back(std::move(t1));
}

int main() {
  ONLY_TO_CONSOLE;
  LOGGER();
  myfunc();
  interact();
  return 0;
}
