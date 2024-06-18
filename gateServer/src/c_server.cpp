#include "asio_io_servicepool.h"
#include "header.h"
#include "http_connection.h"
#include "c_server.h"
CServer::CServer(boost::asio::io_context &ioc, uint16_t &port)
    : acceptor_(ioc, tcp::endpoint(tcp::v4(), port)), ioc_(ioc) {}


		
auto CServer::Start() -> void {
	
  auto self = shared_from_this();
	auto &io_context = AsioIOServicePool::GetInstance()->GetIOService();
	auto new_connection = std::make_shared<HttpConnection>(io_context);
  acceptor_.async_accept(new_connection->GetSocket(), [self,new_connection](beast::error_code ec) {
    try {
      // 出错， 放弃当前连接
			self->ioc_.get_executor();
      if (ec) {
        self->Start();
        return;
      }

      new_connection->Start();

      // 继续监听
      self->Start();
    } catch (std::exception &exp) {
			std::cerr << "exception is " << exp.what() << std::endl;
    }
  });
}