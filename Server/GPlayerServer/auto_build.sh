#!/bin/bash

source scripts/internal/color_config.sh
source scripts/internal/analyze_func.sh
source scripts/internal/build_run.sh
source scripts/internal/auto_clean.sh

if [ $# -lt 1 ]; then
    echo "Need command"
    echo "Example:"
    echo -e "$0 no       ${GREEN_FONT_SET}# 编译${RESET}"
    echo -e "$0 run      ${GREEN_FONT_SET}# 编译并运行,可配置gprof,valgrind分析${RESET}"
    echo -e "$0 test     ${GREEN_FONT_SET}# 编译并运行ctest${RESET}"
    echo -e "$0 coverage ${GREEN_FONT_SET}# 编译并运行ctest,运行lcov,gcovr分析覆盖率${RESET}"
    echo -e "$0 re       ${GREEN_FONT_SET}# 编译Release${RESET}"
    exit 1
elif [ $# -eq 1 ]; then
    command=$1
fi

if [ $command == "no" ]; then
    time build_all Debug
    echo "$0 no run"

elif [ $command == "run" ]; then
    time build_all Debug
    run_demo

elif [ $command == "test" ]; then
    time build_all Debug
    run_ctest
    analyze_valgrind

elif [ $command == "coverage" ]; then
    time build_all Debug
    run_ctest
    coverage_path=coverage
    if [ ! -d coverage ]; then
        mkdir -p ${coverage_path}
    fi

    coverage_lcov
    coverage_gcovr

    tar -zcf ${coverage_path}.tar.gz ${coverage_path}
    zip -rq ${coverage_path}.zip ${coverage_path}
elif [ $command == "clean" ]; then
    rm_all

fi
