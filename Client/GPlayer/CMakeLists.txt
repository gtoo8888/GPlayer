cmake_minimum_required(VERSION 3.5)

project(untitled LANGUAGES CXX)

# set(ENV{GCOV_PREFIX} "/home/yzx/tmp/untitled/build")
# set(ENV{GCOV_PREFIX_STRIP} "5")

SET(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
SET(CMAKE_C_FLAGS "-g -O0 -Wall -W -fprofile-arcs -ftest-coverage")
SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(untitled SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(untitled
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
endif()

target_link_libraries(untitled PRIVATE Qt5::Widgets)

# 添加覆盖率测试目标
add_custom_target(coverage
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    COMMAND lcov --directory . --capture --output-file coverage.info
    # COMMAND lcov --remove coverage.info '/usr/include/c++/7/*' -o coverage_filtered.info
    COMMAND lcov --remove coverage_filtered.info -o coverage.info ;
    '/usr/include/c++/7/*' ;
    '/home/yzx/tmp/untitled/build/untitled_autogen/*';
    '*QtCore*'
    '*QtWidget*'
    COMMAND genhtml coverage.info --output-directory coverage_report
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# lcov --remove /tmp/libreoffice_total.info -o /tmp/libreoffice_filtered.info \
#     '/usr/include/*' \
#     '/usr/lib/*' \
#     '/usr/local/src/libreoffice/*/UnpackedTarball/*' \
#     '/usr/local/src/libreoffice/workdir/*' \
#     '/usr/local/src/libreoffice/instdir/*' \
#     '/usr/local/src/libreoffice/external/*' \
