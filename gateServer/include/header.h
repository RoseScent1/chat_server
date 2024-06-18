#pragma once

//  打印log宏
#define DEBUG_LOG_(str, ...)                                                    \
  {                                                                            \
    std::string log_msg = "[" + std::string(__FILE__) + ":" +                  \
                          std::to_string(__LINE__) + "]\t" +                   \
                          formatString(str, ##__VA_ARGS__);                    \
    std::cout << log_msg << std::endl;                                         \
  }
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/beast.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/core/error.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/ostream.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/dynamic_body.hpp>
#include <boost/beast/http/field.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/status.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/core/ignore_unused.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <memory>
#include <string>
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

enum ErrorCodes {
  Success = 0,
  Error_json = 1001,
  RPCFailed = 1002,
};

template <typename T> struct is_char : std::false_type {};
template <> struct is_char<char *> : std::true_type {};
template <> struct is_char<const char *> : std::true_type {};
template <typename T> constexpr bool is_char_v = is_char<T>::value;

template <typename T, typename... Args>
struct is_ok : std::conditional_t<(is_char_v<std::remove_reference_t<T>> ||
                                   std::is_integral_v<std::remove_reference_t<
                                       T>>)&&is_ok<Args...>::value,
                                  std::true_type, std::false_type> {};
template <typename T>
struct is_ok<T>
    : std::conditional_t<is_char_v<std::remove_reference_t<T>> ||
                             std::is_integral_v<std::remove_reference_t<T>>,
                         std::true_type, std::false_type> {};
template <bool> struct ret_type { using type = std::string; };
template <> struct ret_type<false> {};
// 格式化字符串

template <typename... Args>
typename ret_type<is_ok<Args...>::value>::type formatString(const char *format,
                                                            Args &&...args) {
  auto size = snprintf(nullptr, 0, format, std::forward<Args>(args)...);

  std::string result;
  if (size > 0) {
    result.resize(size);
    snprintf(&result[0], size + 1, format, std::forward<Args>(args)...);
  }
  return result;
}

inline std::string formatString(const char *format) { return format; }
