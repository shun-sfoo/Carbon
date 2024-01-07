#include "Logger.h"
#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <thread>

int download(std::string file) {
  for (int i = 0; i < 10; i++) {
    INFO("Downloading  {} ({}%)", file, i * 10);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
  }
  INFO("Download complete: {}", file);
  return 404;
}

void interact() {
  std::string name;
  std::cin >> name;
  INFO("Hi, {}", name);
}

int main() {
  std::future<int> fret = std::async([&] { return download("hello.zip"); });
  interact();
  while (true) {
    INFO("Waiting for download compelte...");
    auto stat = fret.wait_for(std::chrono::milliseconds(1000)); // wait_until
    if (stat == std::future_status::ready) {
      INFO("Future is ready!!")
      break;
    } // std::future_status::timeout
    INFO("Future not ready!!")
  }
  int ret = fret.get();
  INFO("Download result: {}", ret);
}
