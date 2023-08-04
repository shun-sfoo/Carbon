#pragma once
#include <ParameterCrop.hpp>
#include <functional>

template <typename... Args> class EventHandlerInterface {
public:
  EventHandlerInterface(const EventHandlerInterface &) = delete;
  EventHandlerInterface &operator=(const EventHandlerInterface &) = delete;
  EventHandlerInterface() = default;
  virtual ~EventHandlerInterface() = default;
  virtual void operator()(const Args &...args) = 0;
};

template <typename T, typename Tuple, typename... Args>
class EventHandler : public EventHandlerInterface<Args...> {};

template <typename... TrueArgs, typename... Args>
class EventHandler<void, std::tuple<TrueArgs...>, Args...>
    : public EventHandlerInterface<Args...> {

private:
  std::function<void(TrueArgs...)> m_Handler;

public:
  EventHandler(const EventHandler &event) = delete;
  EventHandler &operator=(const EventHandler &event) = delete;
  ~EventHandler() final = default;
  template <typename Callable> EventHandler(Callable &&func) {
    m_Handler = std::forward<Callable>(func);
  }
  void operator()(const Args &...args) final {
    TupleTake(m_Handler, std::make_tuple(args...));
  }
};

template <typename T, typename... TrueArgs, typename... Args>
class EventHandler<T, std::tuple<TrueArgs...>, Args...>
    : public EventHandlerInterface<Args...> {
  using FunctionPointer = void (T::*)(TrueArgs...);
  using ConstFunctionPointer = void (T::*)(TrueArgs...) const;

private:
  T *m_Receiver;
  std::function<void(T *, TrueArgs...)> m_Handler;

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
