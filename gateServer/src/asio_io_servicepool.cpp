#include "asio_io_servicepool.h"

AsioIOServicePool::AsioIOServicePool(std::size_t size_)
    : io_services_(size_), works_(size_), next_io_service_(0) {

  for (std::size_t i = 0; i < size_; ++i) {
    works_[i] = std::make_unique<Work>(io_services_[i]);
  }

  for (auto &i : io_services_) {
    threads_.emplace_back([&i] { i.run(); });
  }
}

AsioIOServicePool::~AsioIOServicePool() {
	Stop();
  DEBUG_LOG_("AsioIOServicePool destruct");
}

boost::asio::io_context &AsioIOServicePool::GetIOService() {
  auto &service = io_services_[next_io_service_++];
  if (next_io_service_ == io_services_.size()) {
    next_io_service_ = 0;
  }
  return service;
}

void AsioIOServicePool::Stop() {
  for (auto &task : works_) {
    task->get_io_context().stop();
    task.reset();
  }

  for (auto &t : threads_) {
    t.join();
  }
}