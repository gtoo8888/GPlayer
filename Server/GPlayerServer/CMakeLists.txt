cmake_minimum_required(VERSION 3.10)
project(CPlayServer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_COLOR_DIAGNOSTICS ON) # ninja颜色显示

# -rdynamic # 编译选项通知链接器将所有符号添加到动态符号表中。（目的是能够通过使用 dlopen 来实现向后跟踪）
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g3 -ggdb -rdynamic -pthread -Wall -Wextra -fprofile-arcs -ftest-coverage -pg")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3 -ggdb -rdynamic -pthread -Wall -Wextra -fprofile-arcs -ftest-coverage -pg")
    SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Out/Debug/bin)  # 设置可执行文件的位置
    SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Out/Debug/lib) # 设置动态链接库的位置
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -pthread")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -pthread")
    SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Out/Release/bin) 
    SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Out/Release/lib) 
endif()

add_definitions(-w) # 忽略警告

message(STATUS "Build type:                                       ${CMAKE_BUILD_TYPE}")
message(STATUS "C flags, Debug configuration:                     ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "C flags, Release configuration:                   ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "C++ flags, Debug configuration:                   ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "C++ flags, Release configuration:                 ${CMAKE_CXX_FLAGS_RELEASE}")


# REQUIRED：可选字段。表示一定要找到包，找不到的话就立即停掉整个CMake。而如果不指定REQUIRED则CMake会继续执行。

# message(STATUS "CMAKE_PROJECT_NAME                  : ${CMAKE_PROJECT_NAME}")       # 非重要消息；
# message(WARNING "PROJECT_VERSION                    : ${PROJECT_VERSION}")          # CMake 警告, 会继续执行；
# message(AUTHOR_WARNING "PROJECT_VERSION_MAJOR       : ${PROJECT_VERSION_MAJOR}")    # CMake 警告 (dev), 会继续执行；
# message(SEND_ERROR "PROJECT_VERSION_PATCH           : ${PROJECT_VERSION_PATCH}")    # CMake 错误, 继续执行，但是会跳过生成的步骤；
# message(FATAL_ERROR "PROJECT_VERSION_TWEAK          : ${PROJECT_VERSION_TWEAK}")    # CMake 错误, 继续执行，但是会跳过生成的步骤；

file(GLOB SOURCES_IN "src/*/*.cpp")
file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES_IN} ${SOURCES})

include_directories("include")
include_directories("src/PIGG_test")

# MySQL
include_directories(/usr/include/mysql)
target_link_libraries(${PROJECT_NAME} mysqlclient)

# jsoncpp
find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} jsoncpp_static)

# hiredis
# include_directories(/usr/local/include/hiredis)
# find_package(hiredis CONFIG REQUIRED)
# target_link_libraries(${PROJECT_NAME} hiredis)


set_target_properties (${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d)
set_target_properties (${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX _r)


