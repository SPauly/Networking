#pragma once

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "protos/Relay.grpc.pb.h"

#include <iostream>
#include <string>
#include <memory>

namespace SimpleTextingRelay {

class RelayServer final : public RelayService::Service {
 public:
  RelayServer() = delete;
  RelayServer(const std::string& server_address);
  virtual ~RelayServer() = default;

  void Run();

 protected:
  grpc::Status Ping(grpc::ServerContext* ctx, const Id* request,
                    Id* response) override;

 private:
  std::string server_address_;
  grpc::ServerBuilder builder_;
  std::unique_ptr<grpc::Server> server_;
};

}  // namespace SimpleTextingRelay
