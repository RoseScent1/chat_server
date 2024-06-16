#include "http_connection.h"
#include "c_server.h"
CServer::CServer(boost::asio::io_context &ioc, uint16_t &port)
    : acceptor_(ioc, tcp::endpoint(tcp::v4(), port)), ioc_(ioc),
      peer_socket_(ioc) {}
auto CServer::Start() -> void {
  auto self = shared_from_this();
  acceptor_.async_accept(peer_socket_, [self](beast::error_code ec) {
    try {
      // 出错， 放弃当前连接
      if (ec) {
        self->Start();
        return;
      }
      // TODO:成功连接，创建一个HttpConnect类管理此连接的读写操作
      std::make_shared<HttpConnection>(std::move(self->peer_socket_))->Start();

      // 继续监听
      self->Start();
    } catch (std::exception &exp) {
			std::cerr << "exception is " << exp.what() << std::endl;
    }
  });
}