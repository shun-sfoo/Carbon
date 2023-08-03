#pragma once
#include <functional>
#include <tuple>

template <typename Subset, typename Superset> struct is_subset_of;

template <typename T, typename... Subset, typename U, typename... Superset>
struct is_subset_of<std::tuple<T, Subset...>, std::tuple<U, Superset...>> {
  constexpr static bool value =
      is_subset_of<std::tuple<T, Subset...>, std::tuple<Superset...>>::value;
};

template <typename T, typename... Subset, typename... Superset>
struct is_subset_of<std::tuple<T, Subset...>, std::tuple<T, Superset...>> {
  constexpr static bool value =
      is_subset_of<std::tuple<Subset...>, std::tuple<Superset...>>::value;
};

template <typename... Subset>
struct is_subset_of<std::tuple<Subset...>, std::tuple<>> {
  constexpr static bool value = false;
};

template <typename... Superset>
struct is_subset_of<std::tuple<>, std::tuple<Superset...>> {
  constexpr static bool value = true;
};

template <> struct is_subset_of<std::tuple<>, std::tuple<>> {
  constexpr static bool value = true;
};

template <int... Args> struct int_list {
  using type = int_list<Args...>;
  template <typename Tuple>
  constexpr static auto maketuple(const Tuple &target) {
    return std::make_tuple(std::get<Args>(target)...);
  }
};

template <int N, typename list> struct list_prepend;

template <int N, int... Args> struct list_prepend<N, int_list<Args...>> {
  using result = int_list<N, Args...>;
};

template <int N, typename T, typename Tuple> struct find_next_index;

template <int N, typename T, typename U, typename... Args>
struct find_next_index<N, T, std::tuple<U, Args...>> {
  constexpr static int value =
      find_next_index<N - 1, T, std::tuple<Args...>>::value + 1;
};

template <typename T, typename U, typename... Args>
struct find_next_index<-1, T, std::tuple<U, Args...>> {
  constexpr static int value =
      find_next_index<-1, T, std::tuple<Args...>>::value + 1;
};

template <typename T, typename... Args>
struct find_next_index<-1, T, std::tuple<T, Args...>> {
  constexpr static int value = 0;
};

template <int N, typename Subset, typename Superset> struct find_all_index;

template <int N, typename T, typename... Subset, typename... Superset>
struct find_all_index<N, std::tuple<T, Subset...>, std::tuple<Superset...>> {
  using value = typename list_prepend<
      find_next_index<N, T, std::tuple<Superset...>>::value,
      typename find_all_index<
          find_next_index<N, T, std::tuple<Superset...>>::value,
          std::tuple<Subset...>, std::tuple<Superset...>>::value>::result;
};

template <int N, typename... Superset>
struct find_all_index<N, std::tuple<>, std::tuple<Superset...>> {
  using value = int_list<>;
};

template <typename... Subset, typename... Superset>
constexpr auto TupleTake(const std::function<void(Subset...)> &func,
                         const std::tuple<Superset...> &target) {

  /* static_assert( */
  /*     is_subset_of<std::tuple<Subset...>, std::tuple<Superset...>>::value, */
  /*     "slot function parameters and signal parameters do not match"); */
  if constexpr (is_subset_of<std::tuple<Subset...>,
                             std::tuple<Superset...>>::value) {
    using index = typename find_all_index<-1, std::tuple<Subset...>,
                                          std::tuple<Superset...>>::value;
    std::apply(func, index::maketuple(target));
  }
}
