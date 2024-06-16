#include "logic_system.h"
#include "header.h"
#include "http_connection.h"
#include "verify_grpc_client.h"
auto LogicSystem::RegGet(std::string url, HttpHandler handler) -> void {
  DEBUG_LOG_("RegGet,url:%s", url.c_str());
  get_handlers_[url] = handler;
}

auto LogicSystem::RegPost(std::string url, HttpHandler handler) -> void {
  DEBUG_LOG_("RegPost,url:%s", url.c_str());
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
  RegPost("/get_verifycode", [](std::shared_ptr<HttpConnection> connection) {
    auto body_str =
        boost::beast::buffers_to_string(connection->request_.body().data());
    DEBUG_LOG_("receive body is %s", body_str.c_str());
    connection->response_.set(http::field::content_type, "text/json");
    Json::Value root;
    Json::Reader reader;
    Json::Value src_root;
    bool parse_success = reader.parse(body_str, src_root);
    if (!parse_success) {
      DEBUG_LOG_("Failed to parse JSON data!");
      root["error"] = ErrorCodes::Error_json;
      std::string json_str = root.toStyledString();
      beast::ostream(connection->response_.body()) << json_str;
      return true;
    }

    auto email = src_root["email"].asString();
    auto rsp = VerifyGrpcClient::GetInstance()->GetVerifyCode(email);
    DEBUG_LOG_("get email is %s,rsp errcode = %d", email.c_str(), rsp.error());
    root["error"] = rsp.error();
    root["email"] = src_root["email"];
    std::string json_str = root.toStyledString();
    beast::ostream(connection->response_.body()) << json_str;
    return true;
  });
}
bool LogicSystem::HandleGet(std::string path,
                            std::shared_ptr<HttpConnection> con) {
  if (get_handlers_.find(path) == get_handlers_.end()) {
    DEBUG_LOG_("get handlers not found, url:%s", path.c_str());
    return false;
  }

  get_handlers_[path](con);
  return true;
}

bool LogicSystem::HandlePost(std::string path,
                             std::shared_ptr<HttpConnection> con) {
  if (post_handlers_.find(path) == post_handlers_.end()) {
    DEBUG_LOG_("post handlers not found, url:%s", path.c_str());
    return false;
  }
  post_handlers_[path](con);

  return true;
}