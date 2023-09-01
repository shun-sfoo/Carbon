#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

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
  tpool.push_back(std::move(t1));
}

int main() {
  myfunc();
  interact();
  return 0;
}
