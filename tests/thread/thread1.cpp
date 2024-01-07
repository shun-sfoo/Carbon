#include "Logger.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

std::vector<std::thread> pool;
using namespace carbon;

void time1() {
  auto t0 = std::chrono::steady_clock::now();
  auto t1 = t0 + std::chrono::seconds(30);
  auto dt = t1 - t0;
  int64_t sec = std::chrono::duration_cast<std::chrono::seconds>(dt).count();
}

void spend() {
  auto t0 = std::chrono::steady_clock::now();
  for (volatile int i = 0; i < 10000000; i++)
    ;
  auto t1 = std::chrono::steady_clock::now();
  auto dt = t1 - t0;
  int64_t ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(dt).count();
  using double_ms = std::chrono::duration<double, std::milli>;
  double dms = std::chrono::duration_cast<double_ms>(dt).count();
  INFO("time elapsed: {}", ms);
  INFO("time elapsed: {}", dms);
}

void sleep1() { std::this_thread::sleep_for(std::chrono::milliseconds(400)); }

void sleep2() {
  auto t = std::chrono::steady_clock::now() + std::chrono::milliseconds(400);
  std::this_thread::sleep_until(t);
}

void download(std::string file) {
  LOG_LEVEL_INFO;
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
  pool.push_back(std::move(t1));
}

int main() {
  ONLY_TO_CONSOLE;
  LOGGER();
  myfunc();
  interact();
  for (auto &t : pool) {
    t.join();
  }
}
