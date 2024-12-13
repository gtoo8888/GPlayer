#! /bin/bash

if [ $# -lt 1 ];then
    echo "Need path!! Example:"
    echo "$0 test"
    exit 1
elif [ $# -eq 1 ];then
    format_path=$1
fi

# clang-format -style=file -i utility/utilities.cpp

# find test ! -path "*/new_src/* -name "*.cpp"
find $format_path -name "*.cpp" -exec clang-format -style=file -i {} \;
