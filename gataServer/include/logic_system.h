#pragma once
#include "header.h"
#include "singleton.h"
class HttpConnection;
using HttpHandler = std::function<void(std::shared_ptr<HttpConnection>)>;
class LogicSystem : public Singleton<LogicSystem> {
  friend class Singleton<LogicSystem>;
  bool f(std::shared_ptr<HttpConnection> connection);

public:
  ~LogicSystem() = default;
  auto HandleGet(std::string, std::shared_ptr<HttpConnection>) -> bool;
  auto HandlePost(std::string, std::shared_ptr<HttpConnection>) -> bool;
  auto RegGet(std::string, HttpHandler) -> void;
  auto RegPost(std::string, HttpHandler) -> void;

private:
  LogicSystem();

  // post 和 get的回调函数 key为路由
  std::map<std::string, HttpHandler> post_handlers_;
  std::map<std::string, HttpHandler> get_handlers_;
};