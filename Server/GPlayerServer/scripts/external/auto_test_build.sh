#!/bin/bash
set -e

BLACK_FONT_SET='\033[30m'
RED_FONT_SET='\033[31m'
GREEN_FONT_SET='\033[32m'
BLUE_FONT_SET='\033[33m'
PURPLE_FONT_SET='\033[34m'
DEEP_GREEN_FONT_SET='\033[35m'
WHITE_FONT_SET='\033[36m'
RESET='\033[0m'

run_command() {
    echo -e "${BLUE_FONT_SET}now run: $1${RESET}"
    cmd="bash ./$1"
    if ! eval $cmd >/dev/null 2>&1; then
        echo -e "${BLUE_FONT_SET}fail: $1${RESET}"
        exit 1
    else
        echo -e "${BLUE_FONT_SET}success: $1${RESET}"
    fi
}

commands=(
    "./auto_build.sh no"
    "./auto_build.sh run"
    "./auto_build.sh test"
    "./auto_build.sh coverage"
    "./auto_build.sh re"
    "./auto_build.sh docs"
)

# cd ..
for cmd in "${commands[@]}"; do
    run_command "$cmd"
done
