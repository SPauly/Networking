#include "simple_server.h"

namespace SimpleTextingService {

TextingServer::TextingServer(const std::string& server_address)
    : server_address_(server_address) {}

grpc::Status TextingServer::Ping(grpc::ServerContext* ctx, const Id* request,
                                 Id* response) {
  response->set_id(request->id());
  std::cout << "[PING] Received request with ID: " << request->id() << "\n";
  return grpc::Status::OK;
}

void TextingServer::Run() {
  builder_.AddListeningPort(server_address_, grpc::InsecureServerCredentials());
  builder_.RegisterService(this);
  server_ = builder_.BuildAndStart();
  std::cout << "Server listening on " << server_address_ << "\n";
  server_->Wait();
}

}  // namespace SimpleTextingService

int main() {
  SimpleTextingService::TextingServer server("0.0.0.0:50000");
  server.Run();
  return 0;
}