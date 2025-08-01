#!/bin/bash

# 切换到 lm3s6965evb 板级目录
BOARD_DIR="/home/chengxin/qemu-projects/board/lm3s6965evb"
START_SCRIPT="start_lm3s6965evb.sh"

# 检查板级目录是否存在
if [ ! -d "$BOARD_DIR" ]; then
    echo "错误: 板级目录 $BOARD_DIR 不存在"
    exit 1
fi

# 切换到板级目录
cd "$BOARD_DIR" || exit 1

# 检查模拟脚本是否存在
if [ ! -f "$START_SCRIPT" ]; then
    echo "错误: 模拟脚本 $START_SCRIPT 不存在"
    exit 1
fi

# 执行模拟脚本
echo "进入 $BOARD_DIR，执行 $START_SCRIPT..."
./"$START_SCRIPT"