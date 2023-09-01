#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

std::vector<std::thread> pool;

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
  std::cout << "time elapsed: " << ms << " ms" << std::endl;
  std::cout << "time elapsed: " << dms << " ms" << std::endl;
}

void sleep1() { std::this_thread::sleep_for(std::chrono::milliseconds(400)); }

void sleep2() {
  auto t = std::chrono::steady_clock::now() + std::chrono::milliseconds(400);
  std::this_thread::sleep_until(t);
}

void download(std::string file) {
  for (int i = 0; i < 10; i++) {
    std::cout << "Downloading " << file << " (" << i * 10 << "%)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
  }
  std::cout << "Download complete: " << file << std::endl;
}

void interact() {
  std::string name;
  std::cin >> name;
  std::cout << "Hi, " << name << std::endl;
}

void myfunc() {
  std::thread t1([&] { download("hello.zip"); });
  pool.push_back(std::move(t1));
}

int main() {
  myfunc();
  interact();
  for (auto &t : pool) {
    t.join();
  }
}
