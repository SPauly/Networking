#include "SimpleRelay/relay_server.h"

namespace SimpleTextingRelay {

RelayServer::RelayServer(const std::string& server_address)
    : server_address_(server_address) {}

grpc::Status RelayServer::Ping(grpc::ServerContext* ctx, const Id* request,
                               Id* response) {
  response->set_id(42);
  std::cout << "[PING] Received request with ID: " << request->id() << "\n";
  return grpc::Status::OK;
}

void RelayServer::Run() {
  builder_.AddListeningPort(server_address_, grpc::InsecureServerCredentials());
  builder_.RegisterService(this);
  server_ = builder_.BuildAndStart();
  std::cout << "Server listening on " << server_address_ << "\n";
  server_->Wait();
}

}  // namespace SimpleTextingRelay