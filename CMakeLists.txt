cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


project(BasicNetworking)


############################################
# Add gRPC
############################################

option(BUILD_GRPC "Build gRPC" ON)
option(USE_SYSTEM_GRPC "Use system installed gRPC" OFF)

if(USE_SYSTEM_GRPC)
  message(STATUS "Using system-installed gRPC")
  # Find system-installed gRPC
  find_package(gRPC CONFIG REQUIRED)
elseif(BUILD_GRPC)
  message(STATUS "Building gRPC from source")
  
  include(cmake/grpc.cmake)
else()  
  include(cmake/grpc_helpers.cmake)
endif()



