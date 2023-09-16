#include "logger.h"

using namespace carbon;

int main() {
  ONLY_TO_CONSOLE;
  LOG_LEVEL_TRACE;
  TRACE("TRACE");
  DEBUG("DEBUG");
  INFO("INFO");
  WARN("WARN");
  DEBUG("3 {} ", 1);
}
