#pragma once
#include "config_mgr.h"

#include "grpcpp/client_context.h"

#include "header.h"
#include "message.grpc.pb.h"
#include "message.pb.h"
#include "rpc_pool.h"
#include "singleton.h"
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>
#include <memory>
using grpc::ClientContext;
using grpc::Status;
using message::GetVerifyReq;
using message::GetVerifyRsp;

class VerifyGrpcClient : public Singleton<VerifyGrpcClient> {
  friend class Singleton<VerifyGrpcClient>;

public:
  GetVerifyRsp GetVerifyCode(std::string email) {
    ClientContext context;
    GetVerifyRsp rsp;
    GetVerifyReq req;
    req.set_email(email);
    auto stub = pool_->GetStub();
    Status status = stub->GetVerifyCode(&context, req, &rsp);

    // 获取验证码失败
    if (!status.ok()) {
      rsp.set_error(ErrorCodes::RPCFailed);
    }

    pool_->ReturnStub(std::move(stub));
    return rsp;
  }

private:
  VerifyGrpcClient() {
    auto &global_config = ConfigMgr::Instance();
    auto host = global_config["VerifyServer"]["host"];
    auto port = global_config["VerifyServer"]["Port"];
		pool_.reset(new RPCConnectPool(host,port));
  }
  std::unique_ptr<RPCConnectPool> pool_;
};