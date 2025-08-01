#!/bin/bash
FIRMWARE="/home/chengxin/qemu-projects/build/lm3s6965evb/hello_lm3s6965evb.elf"

if [ ! -f "$FIRMWARE" ]; then
    echo "错误: 未找到固件文件 $FIRMWARE" >&2
    exit 1
fi

# 静默启动 QEMU，只显示程序输出
echo "启动 LM3S6965EVB 模拟器..."
/usr/local/bin/qemu-system-arm \
    -M lm3s6965evb \
    -nographic \
    -serial stdio \
    -kernel "$FIRMWARE" \
    -cpu cortex-m3 \
    -monitor none \
    -d guest_errors  # 禁用所有调试输出