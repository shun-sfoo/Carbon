#pragma once

template <int N> struct binary {
  static constexpr int value = binary<N / 10>::value << 1 | N % 10;
};

template <> struct binary<0> {
  static constexpr int value = 0;
};
