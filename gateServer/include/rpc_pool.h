#pragma once
#include "message.grpc.pb.h"
#include <atomic>
#include <condition_variable>
#include <queue>
class RPCConnectPool {

public:
  RPCConnectPool(std::string host, std::string port);
  ~RPCConnectPool();

  // 要关闭了，唤醒所有线程，让他们退出
  void Close() {
    stop_ = true;
    cv_.notify_all();
  }

  std::unique_ptr<message::VerifyService::Stub> GetStub();
	
  void ReturnStub(std::unique_ptr<message::VerifyService::Stub> context);

private:
  std::atomic<bool> stop_;
  std::string host_;
  std::string port_;
  std::queue<std::unique_ptr<message::VerifyService::Stub>> stubs_;
  std::condition_variable cv_;
  std::mutex mutex_;
};