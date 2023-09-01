#pragma once
#include "ParameterCrop.hpp"

// 函数类型擦除相关
template <typename... Args> class EventHandlerInterface {
public:
  EventHandlerInterface(const EventHandlerInterface &) = delete;

  EventHandlerInterface &operator=(const EventHandlerInterface &event) = delete;

  EventHandlerInterface() = default;

  virtual ~EventHandlerInterface() = default;

  // 重载()是为了方便之后调用
  virtual void operator()(const Args &...args) = 0;
};

// 提供两种特化,不用存储this指针的一类
// 另一类如类成员函数是需要对象指针的,所以把类对象指针也存起来
template <typename T, typename Tuple, typename... Args>
class EventHandler : public EventHandlerInterface<Args...> {};

template <typename... SlotArgs, typename... Args>
class EventHandler<void, std::tuple<SlotArgs...>, Args...>
    : public EventHandlerInterface<Args...> {
private:
  std::function<void(SlotArgs...)> m_Handler;

public:
  EventHandler(const EventHandler &event) = delete;

  EventHandler &operator=(const EventHandler &event) = delete;

  ~EventHandler() final = default;

  template <typename Callable> explicit EventHandler(Callable &&func) {
    m_Handler = std::forward<Callable>(func);
  }

  void operator()(const Args &...args) final {
    TupleTake(m_Handler, std::make_tuple(args...));
  }
};

template <typename T, typename... SlotArgs, typename... Args>
class EventHandler<T, std::tuple<SlotArgs...>, Args...>
    : public EventHandlerInterface<Args...> {
  using FunctionPointer = void (T::*)(SlotArgs...);
  using ConstFunctionPointer = void (T::*)(SlotArgs...) const;

private:
  T *m_Receiver;
  std::function<void(T *, SlotArgs...)> m_Handler;

public:
  EventHandler(const EventHandler &event) = delete;

  EventHandler &operator=(const EventHandler &event) = delete;

  ~EventHandler() final = default;

  EventHandler(T *receiver, FunctionPointer handler) {
    m_Receiver = receiver;
    m_Handler = handler;
  }

  EventHandler(T *receiver, ConstFunctionPointer handler) {
    m_Receiver = receiver;
    m_Handler = handler;
  }

  void operator()(const Args &...args) final {
    TupleTake(m_Handler, std::make_tuple(m_Receiver, args...));
  }
};
