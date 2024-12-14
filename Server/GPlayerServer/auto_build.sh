#!/bin/bash

ROOT_DIR=$PWD
RELEASE_FILE=/bin/release/PIGG_webserve
# 使用makefile编译
# chmod +x makefile
# make server


rm -rf build && mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j${nproc}
cd - > /dev/null 2>1&



