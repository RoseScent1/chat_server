#include "logic_system.h"
#include "header.h"
#include "http_connection.h"

auto LogicSystem::RegGet(std::string url, HttpHandler handler) -> void {
  DEBUG_LOG("RegGet,url:%s", url.c_str());
  get_handlers_[url] = handler;
}

auto LogicSystem::RegPost(std::string url, HttpHandler handler) -> void {
  DEBUG_LOG("RegPost,url:%s", url.c_str());
  post_handlers_[url] = handler;
}

LogicSystem::LogicSystem() {
  RegGet("/get_test", [](std::shared_ptr<HttpConnection> connection) -> void {
    beast::ostream(connection->response_.body()) << "receive get_test req";
    int i = 0;
    for (auto &elem : connection->get_params_) {
      ++i;
      beast::ostream(connection->response_.body())
          << "param:" << i << "{" << elem.first << "," << elem.second << "} ";
    }
  });
  RegPost("/get_varifycode", [](std::shared_ptr<HttpConnection> connection) {
    auto body_str =
        boost::beast::buffers_to_string(connection->request_.body().data());
    DEBUG_LOG("receive body is %s", body_str.c_str());
    connection->response_.set(http::field::content_type, "text/json");
    Json::Value root;
    Json::Reader reader;
    Json::Value src_root;
    bool parse_success = reader.parse(body_str, src_root);
    if (!parse_success) {
      DEBUG_LOG("Failed to parse JSON data!");
      root["error"] = ErrorCodes::Error_json;
      std::string jsonstr = root.toStyledString();
      beast::ostream(connection->response_.body()) << jsonstr;
      return true;
    }

    auto email = src_root["email"].asString();
    DEBUG_LOG("email is %s", email.c_str());
    root["error"] = 0;
    root["email"] = src_root["email"];
    std::string jsonstr = root.toStyledString();
    beast::ostream(connection->response_.body()) << jsonstr;
    return true;
  });
}
bool LogicSystem::HandleGet(std::string path,
                            std::shared_ptr<HttpConnection> con) {
  if (get_handlers_.find(path) == get_handlers_.end()) {
    DEBUG_LOG("get handlers not found, url:%s", path.c_str());
    return false;
  }

  get_handlers_[path](con);
  return true;
}

bool LogicSystem::HandlePost(std::string path,
                             std::shared_ptr<HttpConnection> con) {
  if (post_handlers_.find(path) == post_handlers_.end()) {
    DEBUG_LOG("post handlers not found, url:%s", path.c_str());
    return false;
  }
  post_handlers_[path](con);

  return true;
}