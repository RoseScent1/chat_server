cmake_minimum_required(VERSION 3.8.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(test_redis)
# <------------ add hiredis dependency --------------->
set(HIREDIS_PATH "/home/cwl/Desktop/chat/lib/hiredis-dir/")
set(REDIS_PLUS_PLUS_PATH "/home/cwl/Desktop/chat/lib/redis-plus-plus-dir/")


# 找到 hiredis 的头文件和库文件
find_path(HIREDIS_HEADER hiredis PATHS ${HIREDIS_PATH}/include)
find_library(HIREDIS_LIB hiredis PATHS ${HIREDIS_PATH}/lib)

# 找到 redis-plus-plus 的头文件和库文件
find_path(REDIS_PLUS_PLUS_HEADER sw/redis++ PATHS ${REDIS_PLUS_PLUS_PATH}/include)
find_library(REDIS_PLUS_PLUS_LIB redis++ PATHS ${REDIS_PLUS_PLUS_PATH}/lib)
message(${REDIS_PLUS_PLUS_PATH})
# 确保找到了头文件和库文件
if(NOT (HIREDIS_HEADER AND HIREDIS_LIB AND REDIS_PLUS_PLUS_HEADER AND REDIS_PLUS_PLUS_LIB))
    message(FATAL_ERROR "Cannot find hiredis or redis-plus-plus in ${YOUR_LOCATION}")
endif()

# 添加头文件路径到你的项目
include_directories(${HIREDIS_HEADER} ${REDIS_PLUS_PLUS_HEADER})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
add_executable(test_redis ./test.cpp)
# 在你的项目中链接这些库
target_link_libraries(test_redis ${HIREDIS_LIB} ${REDIS_PLUS_PLUS_LIB})
