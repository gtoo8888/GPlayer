#! /bin/bash
set -e

function build_all() {
    build_type=$1
    run_test=$2
    if [ -z "$run_test" ]; then
        run_test=ON
    fi
    core_num=8
    # out="VERBOSE=1"
    out=""
    echo -e "${RED_FONT_SET}build_type:$build_type ${RESET}"
    rm -rf build && rm -rf archives && mkdir build && cd build
    # rm -rf archives && cd build # 现在CMake已经稳定，不删除build，做增量编译
    if [ $build_type == "All" ]; then
        cmake -G "Unix Makefiles" .. -DCMAKE_BUILD_TYPE=Debug
        make -j${core_num} ${out}
        cmake -G "Unix Makefiles" .. -DCMAKE_BUILD_TYPE=Release
        make -j${core_num} ${out}
    elif [ $build_type == "Debug" ]; then
        cmake -G "Unix Makefiles" .. -DCMAKE_BUILD_TYPE=Debug
        make -j${core_num} ${out}
    elif [ $build_type == "Release" ]; then
        cmake -G "Unix Makefiles" .. -DCMAKE_BUILD_TYPE=Release
        make -j${core_num} ${out}
    fi
    cd - 1>/dev/null
}


function run_demo() {
    exe_file=./archives/Debug/bin/GXCodec_d
    # config_file=config/
    ${exe_file} /date_sdb/soft/0_my_test/7_ffmpeg/test_video/test1.mp4
    # ${exe_file} ${config_file}
}


function run_ctest() {
    cd build
    cp -rf ../config/usbdev .
    cp -rf ../config/PinToPinGpioConfig .
    ctest
    cd -
}
