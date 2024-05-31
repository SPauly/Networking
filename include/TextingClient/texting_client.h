#pragma once

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "simple_texting_service.grpc.pb.h"

#include <iostream>
#include <string>
#include <memory>

namespace SimpleTextingService {
class TextingClient {
 public:
  TextingClient() = delete;
  TextingClient(const std::string& server_address);
  virtual ~TextingClient() = default;

  void Run();

  grpc::Status Ping(uint32_t id, SimpleTextingService::Id& response);

 private:
  std::string server_address_;
  std::shared_ptr<grpc::Channel> channel_;
  std::unique_ptr<TextingService::Stub> stub_;
};
}  // namespace SimpleTextingService
