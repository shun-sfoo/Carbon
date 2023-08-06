#pragma once

#include <EventHandler.hpp>
#include <functional>
#include <map>
#include <mutex>
#include <shared_mutex>

template <typename... Args> class Event {
  struct Address {
    void *object;
    void *function;

    Address(void *object, void *function)
        : object(object), function(function) {}

    bool operator<(const Address &other) const { return other.object < object; }

    bool operator==(const Address &other) const {
      return other.object == object && other.function == function;
    }
  };

  using Handler = EventHandlerInterface<Args...> *;

private:
  std::map<Address, Handler> m_Handlers;
  mutable std::shared_mutex m_Mutex;

public:
  // 普通函数，和静态函数的接口
  template <typename... TrueArgs> void Attach(void (*func)(TrueArgs...)) {
    // 这里的handler是一个函数指针,所以不需要存储this指针,所以object指针为nullptr
    // reinterpret_cast<void *>(handler)是为了把函数指针转换为void指针
    std::unique_lock<std::shared_mutex> lock(m_Mutex);
    Address address(nullptr, reinterpret_cast<void *>(func));
    if (!m_Handlers.count(address)) {
      Handler handler =
          new EventHandler<void, std::tuple<TrueArgs...>, Args...>(func);
      m_Handlers.insert(std::make_pair(address, handler));
      ;
    }
  }

  // 类成员函数的接口
  template <typename T, typename... TrueArgs>
  void Attach(T *object, void (T::*func)(TrueArgs...)) {
    // 因为这个类成员函数指针比较特殊，所以func里面其实存的不是地址
    // 也不能直接转换为void*只能通过特殊手段取出来了,而且不同的编译器可能不一样，
    // 具体来说类成员函数指针可能包含两部分，例如在gcc下面是这样的16个字节，前八个字节代表函数地址
    // 后八个字节代表类的地址，所以这里需要把前八个字节取出来，然后转换为void*，这样才能存储
    // MSVC则是直接八个字节，而我们只是把它当作一个标识，起到区分作用就好了
    std::unique_lock<std::shared_mutex> lock(m_Mutex);
    void *buffer = nullptr;
    memcpy(&buffer, &func, sizeof(void *));
    Address address = Address(object, buffer);

    if (!m_Handlers.count(address)) {
      Handler handler =
          new EventHandler<T, std::tuple<TrueArgs...>, Args...>(object, func);
      m_Handlers.insert(std::make_pair(address, handler));
    }
  }

  // 类成员函数的接口 const
  template <typename T, typename... TrueArgs>
  void Attach(T *object, void (T::*func)(TrueArgs...) const) {
    std::unique_lock<std::shared_mutex> lock(m_Mutex);
    void *buffer = nullptr;
    memcpy(&buffer, &func, sizeof(void *));
    Address address = Address(object, buffer);
    if (!m_Handlers.count(address)) {
      Handler handler =
          new EventHandler<T, std::tuple<TrueArgs...>, Args...>(object, func);
      m_Handlers.insert(std::make_pair(address, handler));
    }
  }

  // 仿函数的接口
  template <typename T, typename Lambda>
  void Attach(T *object, Lambda &&lambda) {
    std::unique_lock<std::shared_mutex> lock(m_Mutex);
    ImitationFunctionHelper(object, std::forward<Lambda>(lambda),
                            &Lambda::operator());
  }

private:
  template <typename T, typename U, typename... TrueArgs>
  // 这个函数是用来提取仿函数参数的
  void ImitationFunctionHelper(T *object, U &&t,
                               void (std::decay_t<U>::*func)(TrueArgs...)) {
    void *buffer = nullptr;
    memcpy(&buffer, &func, sizeof(void *));
    Address address = Address(object, buffer);
    std::function<void(TrueArgs...)> f = std::forward<U>(t);
    Handler handler =
        new EventHandler<void, std::tuple<TrueArgs...>, Args...>(std::move(f));
    m_Handlers.insert(std::make_pair(address, handler));
  }

  template <typename T, typename U, typename... TrueArgs>
  void ImitationFunctionHelper(T *object, U &&t,
                               void (std::decay_t<U>::*func)(TrueArgs...)
                                   const) {
    void *buffer = nullptr;
    memcpy(&buffer, &func, sizeof(void *));
    Address address = Address(object, buffer);
    std::function<void(TrueArgs...)> f = std::forward<U>(t);
    Handler handler =
        new EventHandler<void, std::tuple<TrueArgs...>, Args...>(std::move(f));
    m_Handlers.insert(std::make_pair(address, handler));
  }

public:
  template <typename... TrueArgs> void Detach(void (*func)(TrueArgs...)) {
    std::unique_lock<std::shared_mutex> lock(m_Mutex);
    Address address(nullptr, reinterpret_cast<void *>(func));
    if (m_Handlers.count(address)) {
      delete m_Handlers[address];
      m_Handlers.erase(address);
    }
  }

  template <typename T, typename... TrueArgs>
  void Detach(T *object, void (T::*func)(TrueArgs...)) {
    std::unique_lock<std::shared_mutex> lock(m_Mutex);
    void *buffer = nullptr;
    memcpy(&buffer, &func, sizeof(void *));
    Address address = Address(object, buffer);
    if (m_Handlers.count(address)) {
      delete m_Handlers[address];
      m_Handlers.erase(address);
    }
  }

  template <typename T, typename... TrueArgs>
  void Detach(T *object, void (T::*func)(TrueArgs...) const) {
    std::unique_lock<std::shared_mutex> lock(m_Mutex);
    void *buffer = nullptr;
    memcpy(&buffer, &func, sizeof(void *));
    Address address = Address(object, buffer);
    if (m_Handlers.count(address)) {
      delete m_Handlers[address];
      m_Handlers.erase(address);
    }
  }

  void Clear() {
    std::unique_lock<std::shared_mutex> lock(m_Mutex);
    for (auto &&handler : m_Handlers) {
      delete handler.second;
    }
    m_Handlers.clear();
  }

  void operator()(const Args &...args) {
    std::shared_lock<std::shared_mutex> lock(m_Mutex);
    for (auto &&handler : m_Handlers) {
      (*handler.second)(args...);
    }
  }

  void Fired(const Args &...args) {
    std::shared_lock<std::shared_mutex> lock(m_Mutex);
    for (auto &&handler : m_Handlers) {
      (*handler.second)(args...);
    }
  }

  ~Event() { Clear(); }
};
