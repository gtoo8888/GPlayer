#! /bin/bash

rm_cmd_show="rm -rfv"
rm_cmd_no="rm -rf"

rm_ide() {
    ${rm_cmd_no} cmake-build-*
    ${rm_cmd_no} .idea
    ${rm_cmd_no} builds
    ${rm_cmd_no} cache

    echo "clean ide ok !!!"
}

rm_build() {
    ${rm_cmd_show} bin
    ${rm_cmd_show} archives
    ${rm_cmd_no} build

    echo "clean build ok !!!"
}

rm_analyze() {
    ${rm_cmd_show} coverage/gcovr
    ${rm_cmd_show} coverage/lcov
    ${rm_cmd_show} coverage/lcov

    echo "clean analyze ok !!!"
}

rm_docs() {
    ${rm_cmd_no} docs/*GPlayerServerDocs
    ${rm_cmd_show} docs/doxygen_log.log

    echo "clean docs ok !!!"
}


rm_others() {
    ${rm_cmd_no} coverage.zip
    ${rm_cmd_no} coverage.tar.gz
    ${rm_cmd_no} code_stats.*

    echo "clean others ok !!!"
}

rm_all() {
    rm_ide
    rm_build
    rm_analyze
    rm_docs
    rm_others
}
