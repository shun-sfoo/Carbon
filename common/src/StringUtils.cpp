#include "StringUtils.h"
#include <array>
#include <cstdio>
#include <memory>

namespace common {

std::string executeCmd(const std::string &cmd) {
  const int bufferSize = 128;
  std::array<char, bufferSize> buffer{};
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
  if (!pipe) {
    return "FAILED";
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

} // namespace common
