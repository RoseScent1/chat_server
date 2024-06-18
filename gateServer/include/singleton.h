#pragma once
#include "header.h"

template <typename T> class Singleton {
protected:
  Singleton() = default;
  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &other) = delete;
  static std::shared_ptr<T> _instance;

public:
  static std::shared_ptr<T> GetInstance() {
    static std::once_flag s_flag;
    std::call_once(s_flag, [&]() {
      _instance = std::shared_ptr<T>(new T);
    });
    return _instance;
  }
  void PrintAddress() { std::cout << _instance.get() << std::endl; }
  ~Singleton() { DEBUG_LOG_("singleton destruct"); }
};

template <typename T> std::shared_ptr<T> Singleton<T>::_instance = nullptr;
