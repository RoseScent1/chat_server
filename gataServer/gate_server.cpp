#include "c_server.h"


int main() {
	try {
		unsigned short port = static_cast<unsigned short>(8080);
		net::io_context ioc{1};
		boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
		signals.async_wait([&ioc](const boost::system::error_code &error, int signal_number) {
			if (error) {
				return ;
			}
			ioc.stop();
		});
		std::make_shared<CServer>(ioc, port)->Start();
		DEBUG_LOG("GateServer start!");
		ioc.run();
	} catch (const std::exception &e) {
		DEBUG_LOG("Error:%s", e.what());
		return EXIT_FAILURE;
	}
}