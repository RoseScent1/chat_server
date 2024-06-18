#pragma once
#include "header.h"

class CServer : public std::enable_shared_from_this<CServer> {
public:
	CServer(boost::asio::io_context &ioc, uint16_t &port);

	// 监听连接请求
	auto Start() -> void;
private:
	tcp::acceptor acceptor_;
	net::io_context &ioc_;
private:

};
