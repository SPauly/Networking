cmake_minimum_required(VERSION 3.8)

project(BasicNetworking C CXX)

set(CMAKE_CXX_STANDARD 17)

############################################
# Add gRPC
############################################

include(FetchContent)

if(MSVC)
  add_definitions(-D_WIN32_WINNT=0x600)
endif()

find_package(Threads REQUIRED)

set(ABSL_ENABLE_INSTALL ON)
set(grpc_BUILD_TESTS OFF)

FetchContent_Declare(
  gRPC
  GIT_REPOSITORY https://github.com/grpc/grpc
  GIT_TAG        v1.63.0  
)
set(FETCHCONTENT_QUIET ON)
FetchContent_MakeAvailable(gRPC)

set(_PROTOBUF_LIBPROTOBUF libprotobuf)
  set(_REFLECTION grpc++_reflection)
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protoc>)
  set(_GRPC_GRPCPP grpc++)
  if(CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
  else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
  endif()

############################################
# Build Proto files
############################################

set(bn_proto_suf_h ".pb.h")
set(bn_prot_suf_cc ".pb.cc")
set(bn_grpc_suf_h ".grpc.pb.h")
set(bn_grpc_suf_cc ".grpc.pb.cc")

set(bn_proto_files
  "${CMAKE_CURRENT_SOURCE_DIR}/protos/Relay.proto"
)

foreach(bn_p_path ${bn_proto_files})

  get_filename_component(bn_p_path ${bn_p_path} ABSOLUTE)
  get_filename_component(bn_p_proto_path "${bn_p_path}" PATH)

  get_filename_component(bn_p_file ${bn_p_path} NAME_WE)
  message("Building protos for: ${bn_p_file}")

  set(bn_proto_gen_h ${CMAKE_CURRENT_SOURCE_DIR}/protos/${bn_p_file}${bn_proto_suf_h})
  set(bn_proto_gen_cc ${CMAKE_CURRENT_SOURCE_DIR}/protos/${bn_p_file}${bn_prot_suf_cc})
  set(bn_grpc_gen_h ${CMAKE_CURRENT_SOURCE_DIR}/protos/${bn_p_file}${bn_grpc_suf_h})
  set(bn_grpc_gen_cc ${CMAKE_CURRENT_SOURCE_DIR}/protos/${bn_p_file}${bn_grpc_suf_cc})

  add_custom_command(
    OUTPUT "${bn_proto_gen_h}" "${bn_proto_gen_cc}" "${bn_grpc_gen_h}" "${bn_grpc_gen_cc}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${CMAKE_CURRENT_SOURCE_DIR}/protos"
      --cpp_out "${CMAKE_CURRENT_SOURCE_DIR}/protos" 
      -I "${bn_p_proto_path}"
      --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
      "${bn_p_path}"
    DEPENDS "${bn_p_path}"
  )

  include_directories(${CMAKE_CURRENT_BINARY_DIR})
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/protos)

  add_library(${bn_p_file}_proto
    ${bn_proto_gen_h}  
    ${bn_proto_gen_cc}
    ${bn_grpc_gen_h}
    ${bn_grpc_gen_cc}
  )

  target_link_libraries(${bn_p_file}_proto
  absl::check
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  )
endforeach()

############################################
# Build SimpleRelay
############################################

add_executable(SimpleRelay
  "${CMAKE_CURRENT_SOURCE_DIR}/SimpleRelay/relay_server.cc"
)

target_include_directories(SimpleRelay
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/protos
)

target_link_libraries(SimpleRelay
  Relay_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)

############################################
# Build TextingClient
############################################

add_executable(TextingClient
  "${CMAKE_CURRENT_SOURCE_DIR}/src/texting_client.cc"
)

target_include_directories(TextingClient
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/protos
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(TextingClient
  Relay_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)


