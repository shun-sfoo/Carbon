#pragma once
#include <functional>
#include <tuple>

// 参数裁剪相关
template <typename Subset, typename Superset> struct is_subset_of;

// 如果子集的元素和父集的元素不一样，那就递归找父集合的下一个元素
template <typename T, typename... Subset, typename U, typename... Superset>
struct is_subset_of<std::tuple<T, Subset...>, std::tuple<U, Superset...>> {
  constexpr static bool value =
      is_subset_of<std::tuple<T, Subset...>, std::tuple<Superset...>>::value;
};

// 如果子集元素和父集元素的一样，那子集就递归寻找下一个元素
template <typename T, typename... Subset, typename... Superset>
struct is_subset_of<std::tuple<T, Subset...>, std::tuple<T, Superset...>> {
  constexpr static bool value =
      is_subset_of<std::tuple<Subset...>, std::tuple<Superset...>>::value;
};

// 递归终止条件，如果子集都找完了，父集还没没空，说明满足子序列关系
template <typename... Superset>
struct is_subset_of<std::tuple<>, std::tuple<Superset...>> {
  constexpr static bool value = true;
};

// 递归终止条件，如果父集找完了子集还没空，说明找不到，不满足子序列关系
template <typename... Subset>
struct is_subset_of<std::tuple<Subset...>, std::tuple<>> {
  constexpr static bool value = false;
};

// 上面有一种临界情况，子集父集都是空，根据模板特化的匹配规则上面两个递归终止条件，都符合，这样的话就会出现编译错误，所以我们要单独处理一下
// 显然两者都找完了，也是符合条件的，这说明子集的最后一个元素也是父集的最后一个元素
template <> struct is_subset_of<std::tuple<>, std::tuple<>> {
  constexpr static bool value = true;
};

// 储存元素
template <int... Args> struct int_list {
  using type = int_list<Args...>;

  // 根据储存的元素，从给定的tuple中进行裁剪
  template <typename Tuple>
  constexpr static auto MakeTupleByList(const Tuple &target) {
    return std::make_tuple(std::get<Args>(target)...);
  }
};

// 头插
template <int N, typename list> struct list_prepend;

template <int N, int... Args> struct list_prepend<N, int_list<Args...>> {
  using result = int_list<N, Args...>;
};

// 查找在目标序列第N个位置之后有没有类型T

// 参数N代表开始查找的起点，T是待查找元素，Tuple是元素集合
template <int N, typename T, typename Tuple> struct find_next_index;

// 首先要从Tuple中进行元素的剔除，剔除掉前N个元素，然后再开始查找
// 这个递归就是在进行剔除
template <int N, typename T, typename U, typename... Args>
struct find_next_index<N, T, std::tuple<U, Args...>> {
  constexpr static int value =
      find_next_index<N - 1, T, std::tuple<Args...>>::value + 1;
};

// 当N为-1的时候表示剔除完毕,开始进行递归查找，由于我们已经判断满足了子序列关系，所以不用担心找不到的情况
template <typename T, typename... Args>
struct find_next_index<-1, T, std::tuple<T, Args...>> {
  constexpr static int value = 0;
};

// 递归终止，找到对应元素
template <typename T, typename U, typename... Args>
struct find_next_index<-1, T, std::tuple<U, Args...>> {
  constexpr static int value =
      find_next_index<-1, T, std::tuple<Args...>>::value + 1;
};

// 找出子序列的函数,N标识从什么位置开始找
template <int N, typename Subset, typename Superset> struct find_all_index;

// 递归寻找
template <int N, typename T, typename... Subset, typename... Superset>
struct find_all_index<N, std::tuple<T, Subset...>, std::tuple<Superset...>> {
  using value = typename list_prepend<
      find_next_index<N, T, std::tuple<Superset...>>::value,
      typename find_all_index<
          find_next_index<N, T, std::tuple<Superset...>>::value,
          std::tuple<Subset...>, std::tuple<Superset...>>::value>::result;
};

// 子集为空，说明全都找完了，递归终止
template <int N, typename... Superset>
struct find_all_index<N, std::tuple<>, std::tuple<Superset...>> {
  using value = int_list<>;
};

// 好了，有了上面的的那些函数，我们来实现TupleTake吧
template <typename... Subset, typename... Superset>
constexpr auto TupleTake(const std::function<void(Subset...)> &func,
                         const std::tuple<Superset...> &target) {
  // static_assert(
  //    is_subset_of<std::tuple<Subset...>, std::tuple<Superset...>>::value,
  //   "slot function parameters and signal parameters do not match");
  if constexpr (is_subset_of<std::tuple<Subset...>,
                             std::tuple<Superset...>>::value) {
    using IndexList = typename find_all_index<-1, std::tuple<Subset...>,
                                              std::tuple<Superset...>>::value;
    std::apply(func, IndexList::MakeTupleByList(target));
  }
}
