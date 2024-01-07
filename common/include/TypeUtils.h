#pragma once
#include <type_traits>

// moder effective c++ item10
// auto val = std::get<toUType(UserInfoFields::uiEmail)>(uInfo);

template <typename E> constexpr auto toUType(E enumrator) noexcept {
  return static_cast<std::underlying_type_t<E>>(enumrator);
}
