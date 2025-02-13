#! /bin/bash
set -e

function coverage_lcov() {
    test_name_date=$1
    lcov_path=coverage/lcov
    lcov_ans_path=${lcov_path}/lcov_ans.log
    data_time_formate=$(date "+%Y-%m-%d %H:%M:%S")

    if [ ! -d ${lcov_path} ]; then
        mkdir -p ${lcov_path}
    fi

    lcov --capture \
        --directory build \
        --output-file ${lcov_path}/${test_name_date}_tmp.info

    lcov --remove ${lcov_path}/${test_name_date}_tmp.info \
        '*/usr/include/*' '*/usr/lib/*' '*/usr/lib64/*' \
        '*/usr/local/include/*' '*/usr/local/lib/*' '*/usr/local/lib64/*' \
        '*/unittest/*' '*/head/*' '*/include/*' '*/src/cJSON.c' \
        --output-file ${lcov_path}/${test_name_date}.info

    genhtml -o ${lcov_path}/${test_name_date}_html_lcov ${lcov_path}/${test_name_date}.info

    echo "lcov_test_time : $data_time_formate" >>$lcov_ans_path
    genhtml -o ${lcov_path}/${test_name_date}_html_lcov ${lcov_path}/${test_name_date}.info | tail -n 2 >>$lcov_ans_path

    rm -rf ${lcov_path}/${test_name_date}.info
    rm -rf ${lcov_path}/${test_name_date}_tmp.info
}

function coverage_gcovr() {
    gcovr_path=coverage/gcovr
    gcovr_html_path=${gcovr_path}/${test_name_date}_html_gcovr
    gcovr_ans_path=${gcovr_path}/gcovr_ans.log
    data_time_formate=$(date "+%Y-%m-%d %H:%M:%S")

    if [ ! -d ${gcovr_html_path} ]; then
        mkdir -p ${gcovr_html_path}
    fi

    function run_gcovr() {
        local output_format=$1
        shift # 移除第一个参数，剩下的$@是额外的参数
        # --filter 需要统计覆盖率的文件夹
        # --exclude 需要排除掉，不统计覆盖率的文件
        gcovr --root . --filter "source/src" --exclude "source/src/cJSON.c" $output_format $@
    }

    run_gcovr ""

    echo "gcovr_test_time : $data_time_formate" >>"$gcovr_ans_path"
    run_gcovr "--print-summary --exclude-unreachable-branches" | tail -n 2 >>"$gcovr_ans_path"

    run_gcovr "--html --html-details" "--output ${gcovr_html_path}/gcovr_coverage.html"

    run_gcovr "--xml-pretty --exclude-unreachable-branches --print-summary" "--output ${gcovr_html_path}/gcovr_coverage.xml"
}

function analyze_gprof() {
    data_time=$(date "+%Y%m%d_%H%M%S")
    gprof_analyze_path=coverage/analyze/gprof
    exe_file=$1
    exe_path=./bin/x86/Debug/${exe_file}
    gprof_flat_profile_filename=gprof_flat_profile_${data_time}_${exe_file}.log
    gprof_call_graph_filename=gprof_call_graph_${data_time}_${exe_file}.log

    if [ ! -d $gprof_analyze_path ]; then
        mkdir -p $gprof_analyze_path
    fi
    # gprof -p test gmon.out > test.log
    gprof --flat-profile $exe_path gmon.out >$gprof_analyze_path/${gprof_flat_profile_filename} # 执行时间统计
    # gprof -q test gmon.out > test.log
    gprof --graph $exe_path gmon.out >$gprof_analyze_path/${gprof_call_graph_filename} # 调用关系统计
    echo "gprof flat profile path: $gprof_analyze_path/${gprof_flat_profile_filename} "
    echo "gprof call graph path: $gprof_analyze_path/${gprof_call_graph_filename}"
}

function analyze_valgrind() {
    data_time=$(date "+%Y%m%d_%H%M%S")
    valgrind_analyze_path=coverage/analyze/valgrind
    exe_file=$1
    exe_path=./bin/x86/Debug/${exe_file}
    valgrind_memcheck_filename=valgrind_memcheck_${data_time}_${exe_file}.log
    valgrind_massif_filename=out.valgrind_${data_time}_${exe_file}.
    valgrind_massif_ms_print_filename=valgrind_massif_ms_print_${data_time}_${exe_file}.log

    if [ ! -d $valgrind_analyze_path ]; then
        mkdir -p $valgrind_analyze_path
    fi

    # 可以调整参数
    sudo valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --show-reachable=yes \
        ${exe_path} >$valgrind_analyze_path/${valgrind_memcheck_filename} 2>&1
    sudo valgrind --tool=massif ${exe_path} >/dev/null 2>&1
    # massif.out.20859 文件名样例
    # 如果有多个massif.out.*会产生不可预知问题
    massif_out_file_old=$(find . -maxdepth 1 -name "massif.out.*")
    massif_out_file="${massif_out_file_old/"out."/${valgrind_massif_filename}}"
    mv $massif_out_file_old $massif_out_file    # 改名
    mv $massif_out_file $valgrind_analyze_path/ # 移动
    ms_print $valgrind_analyze_path/$massif_out_file >$valgrind_analyze_path/${valgrind_massif_ms_print_filename}
    echo "valgrind memcheck path: $valgrind_analyze_path/${valgrind_memcheck_filename}"
    echo "valgrind massif path: $valgrind_analyze_path/${massif_out_file}"
}
