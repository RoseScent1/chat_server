#include "c_server.h"
#include "config_mgr.h"
#include "header.h"
#include <cstdint>
int main() {
  auto global_config = ConfigMgr::Instance();
  std::string gate_port_str = global_config["GateServer"]["Port"];
  uint16_t port = atoi(gate_port_str.c_str());
  try {
    net::io_context ioc{1};
    boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait(
        [&ioc](const boost::system::error_code &error, int signal_number) {
          if (error) {
            return;
          }
          ioc.stop();
        });
    std::make_shared<CServer>(ioc, port)->Start();
    DEBUG_LOG_("GateServer start!");
    ioc.run();
  } catch (const std::exception &e) {
    DEBUG_LOG_("Error:%s", e.what());
    return EXIT_FAILURE;
  }
}