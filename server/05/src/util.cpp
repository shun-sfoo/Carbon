#pragma once
#include <cstdio>

void errif(bool condition, const char *errMsg) {
  if (condition) {
    perror(errMsg);
  }
}
