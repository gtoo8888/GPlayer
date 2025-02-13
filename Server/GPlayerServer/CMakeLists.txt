cmake_minimum_required(VERSION 3.10)
project(GPlayServer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_COLOR_DIAGNOSTICS ON) # ninja颜色显示

# -rdynamic # 编译选项通知链接器将所有符号添加到动态符号表中。（目的是能够通过使用 dlopen 来实现向后跟踪）
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g3 -ggdb -rdynamic -pthread -Wall -Wextra -fprofile-arcs -ftest-coverage -pg")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3 -ggdb -rdynamic -pthread -Wall -Wextra -fprofile-arcs -ftest-coverage -pg")
    SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/archives/Debug/bin) # 设置可执行文件的位置
    SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/archives/Debug/lib) # 设置动态链接库的位置
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -pthread")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -pthread")
    SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/archives/Release/bin)
    SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/archives/Release/lib)
endif()

option(USE_TOP_CMAKE "use top cmake" ON)

add_definitions(-w) # 忽略警告

add_subdirectory(src/GPlayerServer)
add_subdirectory(src/GXCodec)
