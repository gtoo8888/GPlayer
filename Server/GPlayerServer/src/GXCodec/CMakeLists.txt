cmake_minimum_required(VERSION 3.10)

PROJECT(GXCodec)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_COLOR_DIAGNOSTICS ON) # ninja颜色显示

if(USE_TOP_CMAKE)
    message(WARNING "${PROJECT_NAME} USE_TOP_CMAKE")
else()
    message(WARNING "${PROJECT_NAME} NOT USE_TOP_CMAKE")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g3 -ggdb -Wall -Wextra -fprofile-arcs -ftest-coverage -pg")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3 -ggdb -Wall -Wextra -fprofile-arcs -ftest-coverage -pg")
        SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/Debug)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_C_FLAGS_RELEASE -O2)
        set(CMAKE_CXX_FLAGS_RELEASE -O2)
        SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/Release)
    endif()
endif()

message(STATUS "Build type:                                       ${CMAKE_BUILD_TYPE}")
message(STATUS "C flags, Debug configuration:                     ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "C flags, Release configuration:                   ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "C++ flags, Debug configuration:                   ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "C++ flags, Release configuration:                 ${CMAKE_CXX_FLAGS_RELEASE}")

file(GLOB SOURCES "**/*.cpp")
ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES})
include_directories("/usr/local/ffmpeg/include/")
include_directories("include")

link_directories(/usr/local/ffmpeg/lib/)

target_link_libraries(${PROJECT_NAME}
    /usr/local/ffmpeg/lib/libavcodec.so
    /usr/local/ffmpeg/lib/libavformat.so
)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d)
set_target_properties(${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX _r)

# 检查当前是否存在/usr/local/ffmpeg/lib/库目录
# execute_process(
# COMMAND ls /usr/local/ffmpeg/lib/
# OUTPUT_VARIABLE LIST_LIB_FILES
# RESULT_VARIABLE LS_RESULT
# )
# if(LS_RESULT EQUAL 0)
# message(STATUS "Files in /usr/local/ffmpeg/lib/: ${LIST_LIB_FILES}")
# else()
# message(WARNING "Failed to list files in /usr/local/ffmpeg/lib/. Result=${LS_RESULT}")
# endif()