#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "noncopyable.h"

#include <atomic>
#include <spdlog/spdlog.h>
#include <string>

namespace carbon {

class Logger final : noncopyable {
public:
  static Logger &Instance();
  bool init(const std::string &filePath = "logs/carbon.log",
            const std::string &loggerName = "logger",
            spdlog::level::level_enum level = spdlog::level::trace);
  void setLevel(spdlog::level::level_enum level);
  static void onlyToConsole() { m_writeToFile = false; }
  static void onlyToFile() { m_writeToConsole = false; }
  static void shundown() { spdlog::shutdown(); }

private:
  std::atomic<bool> m_isInited{false};
  static std::atomic<bool> m_writeToFile;
  static std::atomic<bool> m_writeToConsole;
};

} // namespace carbon

// spd 带行号的打印，同时输出console和文件
#define TRACE(...)                                                             \
  SPDLOG_LOGGER_TRACE(spdlog::default_logger_raw(), __VA_ARGS__);
#define DEBUG(...)                                                             \
  SPDLOG_LOGGER_DEBUG(spdlog::default_logger_raw(), __VA_ARGS__);
#define INFO(...) SPDLOG_LOGGER_INFO(spdlog::default_logger_raw(), __VA_ARGS__);
#define WARN(...) SPDLOG_LOGGER_WARN(spdlog::default_logger_raw(), __VA_ARGS__);
#define ERROR(...)                                                             \
  SPDLOG_LOGGER_ERROR(spdlog::default_logger_raw(), __VA_ARGS__);
#define CRITICAL(...)                                                          \
  SPDLOG_LOGGER_CRITICAL(spdlog::default_logger_raw(), __VA_ARGS__);           \
  abort();

#define LOG_LEVEL_INFO spdlog::set_level(spdlog::level::info);
#define LOG_LEVEL_DEBUG spdlog::set_level(spdlog::level::debug);
#define LOG_LEVEL_TRACE spdlog::set_level(spdlog::level::trace);
#define LOG_LEVEL_WARN spdlog::set_level(spdlog::level::warn);
#define LOG_LEVEL_ERROR spdlog::set_level(spdlog::level::err);
#define LOG_LEVEL_CRITICAL spdlog::set_level(spdlog::level::critical);

// todo need to improve
// #define LOGGER Logger::Instance().init("Logger", "logs/log.txt",
// spdlog::level::trace);

#define LOGGER(...) Logger::Instance().init(__VA_ARGS__);
#define LOGGER_WITH_NAME(name) Logger::Instance().init("logs/log.txt", name);
#define ONLY_TO_CONSOLE Logger::onlyToConsole();
#define ONLY_TO_FILE Logger::onlyToFile();
