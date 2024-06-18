cmake_minimum_required(VERSION 3.8.0)
project(grpc_proto C CXX)


find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)
set(pb_src ./message.grpc.pb.cc ./message.pb.cc)
link_libraries(gRPC::grpc++)
link_libraries(protobuf)

add_library(grpc_proto ${pb_src})
# target_link_libraries(grpc_proto  gRPC::grpc++ protobuf)
