#include "TextingClient/texting_client.h"

namespace SimpleTextingRelay {

TextingClient::TextingClient(const std::string& server_address)
    : server_address_(server_address) {}

void TextingClient::Run() {
  // Create a channel to the server
  channel_ =
      grpc::CreateChannel(server_address_, grpc::InsecureChannelCredentials());

  // Create a stub to the server
  stub_ = RelayService::NewStub(channel_);

  grpc::ClientContext ctx;
  // Send a ping request to the server

  SimpleTextingRelay::Id pid, rid;

  grpc::Status status = stub_->Ping(&ctx, pid, &rid);

  if (status.ok()) {
    std::cout << "[PING] Received response with ID: " << rid.id() << "\n";
  } else {
    std::cout << "[PING] Error: " << status.error_code() << ": "
              << status.error_message() << "\n";
  }

  while (true) {
    uint32_t id;
    std::cout << "Enter an ID: ";
    std::cin >> id;
    pid.set_id(id);
    grpc::Status status = Ping(id, rid);
    if (status.ok()) {
      std::cout << "[PING] Received response with ID: " << rid.id() << "\n";
    } else {
      std::cout << "[PING] Error: " << status.error_code() << ": "
                << status.error_message() << "\n";
    }
  }
}

grpc::Status TextingClient::Ping(uint32_t id,
                                 SimpleTextingRelay::Id& response) {
  grpc::ClientContext ctx;
  SimpleTextingRelay::Id pid;

  pid.set_id(id);

  return stub_->Ping(&ctx, pid, &response);
}

}  // namespace SimpleTextingRelay

int main() {
  SimpleTextingRelay::TextingClient client("localhost:50000");
  client.Run();
  return 0;
}