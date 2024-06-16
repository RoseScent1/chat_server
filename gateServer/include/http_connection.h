
#pragma once
#include "header.h"
#include "logic_system.h"
#include <chrono>

class HttpConnection : public std::enable_shared_from_this<HttpConnection> {
  friend class LogicSystem;

public:
  HttpConnection(tcp::socket socket);
  // 监听读事件
  auto Start() -> void;

private:
  // 判断是否超时
  auto CheckDeadline() -> void;

  // 发送回复包
  auto WriteResponse() -> void;

  // 处理请求
  auto HandleReq() -> void;

	// 解析url
  void PreParseGetParam();
  tcp::socket socket_;

  // 接受的请求消息
  beast::flat_buffer buffer_{8192};
  http::request<http::dynamic_body> request_;
  // 发送的回复消息
  http::response<http::dynamic_body> response_;

  // 定时判断请求是否超时
  net::steady_timer deadline_{socket_.get_executor(), std::chrono::seconds(60)};

  std::string get_url_;
  std::unordered_map<std::string, std::string> get_params_;
};