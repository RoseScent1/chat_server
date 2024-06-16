#pragma once
#include "grpcpp/channel.h"
#include "grpcpp/client_context.h"
#include "grpcpp/create_channel.h"
#include "grpcpp/security/credentials.h"
#include "header.h"
#include "message.grpc.pb.h"
#include "message.pb.h"
#include "singleton.h"
#include <grpcpp/grpcpp.h>
#include <grpcpp/support/status.h>
#include <memory>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using message::GetVerifyReq;
using message::GetVerifyRsp;
using message::VerifyService;

class VerifyGrpcClient : public Singleton<VerifyGrpcClient> {
  friend class Singleton<VerifyGrpcClient>;

public:
  GetVerifyRsp GetVerifyCode(std::string email) {
    ClientContext context;
    GetVerifyRsp rsp;
    GetVerifyReq req;
    req.set_email(email);
    Status status = stub_->GetVerifyCode(&context, req, &rsp);

    // 获取验证码失败
    if (!status.ok()) {
      rsp.set_error(ErrorCodes::RPCFailed);
    }
    return rsp;
  }

private:
	VerifyGrpcClient() {
		std::shared_ptr<Channel> channel = grpc::CreateChannel("0.0.0.0:50051", grpc::InsecureChannelCredentials());
		stub_ = VerifyService::NewStub(channel);
	}
  std::unique_ptr<VerifyService::Stub> stub_;
};