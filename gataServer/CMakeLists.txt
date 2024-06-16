cmake_minimum_required(VERSION 3.5.0)
project(gataServer VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-std=c++17  -Wall -Werror)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC)


include(CTest)
enable_testing()
include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}/include


)

link_libraries(jsoncpp)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(gataServer gate_server.cpp ${SRC})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
