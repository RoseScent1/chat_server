cmake_minimum_required(VERSION 3.8.0)

project(gateServer VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-std=c++17 -Wall -Werror -Wno-error=unused-variable)

if(POLICY CMP0079)
	cmake_policy(SET CMP0079 NEW)
endif()

set(ABSL_PROPAGATE_CXX_STD ON)


set(CMAKE_PREFIX_PATH /home/cwl/Desktop/chat/lib/grpc_dir)


aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC)
include(CTest)
enable_testing()




include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/proto
)


find_package(Boost REQUIRED COMPONENTS filesystem)
include_directories(${CMAKE_PREFIX_PATH}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)
link_libraries(jsoncpp)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


add_executable(gateServer gate_server.cpp ${SRC})
target_link_libraries(gateServer grpc_proto)
target_link_libraries(gateServer Boost::filesystem)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
add_subdirectory(proto)
add_subdirectory(redis)