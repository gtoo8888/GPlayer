#! /bin/bash
set -e

RESET='\033[0m'

# set
BOLD_SET='\033[1m'       # 加粗
DIM_SET='\033[2m'        # 变暗
UNDERLINED_SET='\033[4m' # 下划线
BLINK_SET='\033[5m'      # 闪烁
MINVERTED_SET='\033[7m'  # 反转,反转前景色和背景色
HIDDEN_SET='\033[8m'     # 隐藏,对密码有用

# font color
BLACK_FONT_SET='\033[30m'
RED_FONT_SET='\033[31m'
GREEN_FONT_SET='\033[32m'
BLUE_FONT_SET='\033[33m'
PURPLE_FONT_SET='\033[34m'
DEEP_GREEN_FONT_SET='\033[35m'
WHITE_FONT_SET='\033[36m'

# background color
BLACK_BACKGROUND_SET='\033[40m'
RED_BACKGROUND_SET='\033[41m'
GREEN_BACKGROUND_SET='\033[42m'
BLUE_BACKGROUND_SET='\033[43m'
PURPLE_BACKGROUND_SET='\033[44m'
DEEP_GREEN_BACKGROUND_SET='\033[45m'
WHITE_BACKGROUND_SET='\033[46m'
