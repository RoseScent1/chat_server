#pragma once
#include "header.h"
#include "singleton.h"
class AsioIOServicePool : public Singleton<AsioIOServicePool> {
  friend Singleton<AsioIOServicePool>;

public:
  using IOService = net::io_context;
  using Work = IOService::work;
  using WorkPtr = std::unique_ptr<Work>;
	
  ~AsioIOServicePool();

  AsioIOServicePool(const AsioIOServicePool &) = delete;
	AsioIOServicePool& operator = (const AsioIOServicePool&) = delete;

	IOService& GetIOService();
	void Stop();
private:

	AsioIOServicePool(std::size_t size_= std::thread::hardware_concurrency());
	std::vector<IOService> io_services_;
	std::vector<WorkPtr> works_;
	std::vector<std::thread> threads_;
	std::size_t next_io_service_;
};