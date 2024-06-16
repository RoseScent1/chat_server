cmake_minimum_required(VERSION 3.8.0)
project(grpc_proto C CXX)





find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)
add_library(grpc_proto ./message.grpc.pb.cc ./message.pb.cc)
target_link_libraries(grpc_proto  gRPC::grpc++ protobuf)
