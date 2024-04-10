#include <cstdint>
#include <format>
#include <iostream>
#include <source_location>
#include <utility>

// X MACRO
#define FOREACH_log_level(f) f(trace) f(debug) f(info) f(critical) f(warning) f(error)

enum class log_level : uint8_t {
#define _FUNCTION(name) name,
  FOREACH_log_level(_FUNCTION)
#undef _FUNCTION
};

std::string log_level_name(log_level lev) {
  switch (lev) {
#define _FUNCTION(name)                                                                                                \
  case log_level::name: return #name;
    FOREACH_log_level(_FUNCTION)
#undef _FUNCTION
  }
  return "unknown";
}

template <typename T> struct with_source_location {
private:
  T m_inner;
  std::source_location m_loc;

public:
  template <typename U>
    requires std::constructible_from<T, U>
  consteval with_source_location(U &&inner, std::source_location loc = std::source_location::current())
      : m_inner(std::forward<U>(inner)), m_loc(std::move(loc)) {}
  constexpr T const &format() const { return m_inner; }
  constexpr std::source_location const &location() const { return m_loc; };
};

static log_level max_level = log_level::info;

template <typename... Args>
void generic_log(log_level lev, with_source_location<std::format_string<Args...>> fmt, Args &&...args) {
  auto const &loc = fmt.location();
  if (lev >= max_level) {
    std::cout << loc.file_name() << ":" << loc.line() << " [ " << log_level_name(lev) << " ] "
              << std::vformat(fmt.format().get(), std::make_format_args(args...)) << "\n";
  }
}

#define LOG(msg)

int main() {
  generic_log(log_level::debug, "{}", 123);
  generic_log(log_level::warning, "{}", "hello");
}
