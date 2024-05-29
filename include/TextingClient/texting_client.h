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

namespace SimpleTextingRelay {}  // namespace SimpleTextingRelay
