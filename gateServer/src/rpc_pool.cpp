#include "rpc_pool.h"
#include <grpcpp/create_channel.h>
#include <mutex>

RPCConnectPool::RPCConnectPool(std::string host, std::string port)
    : host_(host), port_(port) {
  std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(
      host + ":" + port, grpc::InsecureChannelCredentials());

  stubs_.push(message::VerifyService::NewStub(channel));
}

RPCConnectPool::~RPCConnectPool() {
  std::lock_guard<std::mutex> lock(mutex_);
  while (!stubs_.empty()) {
    stubs_.pop();
  }
}

std::unique_ptr<message::VerifyService::Stub> RPCConnectPool::GetStub() {
  std::unique_lock<std::mutex> lock(mutex_);
  cv_.wait(lock, [this] {
    if (stop_) {
      return true;
    }
    return !stubs_.empty();
  });
  if (stop_) {
    return nullptr;
  }
  auto stub = std::move(stubs_.front());
  stubs_.pop();
  return stub;
}

void RPCConnectPool::ReturnStub(
    std::unique_ptr<message::VerifyService::Stub> context) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (stop_) {
    return;
  }
  stubs_.push(std::move(context));
  cv_.notify_one();
}