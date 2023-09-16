#include "logger.h"
#include <chrono>
#include <cstdio>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <vector>

using namespace carbon;

std::atomic<bool> Logger::m_writeToFile{true};
std::atomic<bool> Logger::m_writeToConsole{true};

Logger &Logger::Instance() {
  static Logger log;
  return log;
}

bool Logger::init(const std::string &filePath, const std::string &loggerName,
                  spdlog::level::level_enum level) {

  if (m_isInited) {
    return true;
  }

  if (!m_writeToConsole && !m_writeToFile) {
    printf("Initialized AN EMPTY Logger!\n");
    return true;
  }
  try {

    spdlog::flush_every(std::chrono::seconds(3));

    std::vector<spdlog::sink_ptr> sinks;
    if (m_writeToConsole) {
      auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      sinks.push_back(std::move(sink));
    }

    if (m_writeToFile) {
      auto sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
          filePath, 1024 * 1024 * 5, 5, false);
      sinks.push_back(std::move(sink));
    }

    std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>(
        loggerName, sinks.begin(), sinks.end());
    logger->set_level(level);
    auto formatter = std::make_unique<spdlog::pattern_formatter>();
    formatter->set_pattern(
        "%Y-%m-%d %H:%M:%S [%l] [tid : %t] [%s : %# <%!>] %v");
    logger->set_formatter(std::move(formatter));
    logger->flush_on(level);
    spdlog::set_default_logger(logger);
  } catch (const spdlog::spdlog_ex &ex) {
    printf("Log initialization failed: %s\n", ex.what());
  }
  m_isInited = true;

  return true;
}
