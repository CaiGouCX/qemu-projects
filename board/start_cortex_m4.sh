#!/bin/bash
FIRMWARE="/home/chengxin/qemu-projects/build/lm3s6965/hello_lm3s6965.elf"

if [ ! -f "$FIRMWARE" ]; then
    echo "错误: 未找到固件文件 $FIRMWARE"
    exit 1
fi

echo "启动 LM3S6965EVB 模拟器..."
echo "固件: $FIRMWARE"

/usr/local/bin/qemu-system-arm \
    -M lm3s6965evb \
    -cpu cortex-m3 \
    -nographic \
    -kernel "$FIRMWARE" \
    -serial stdio \
    -d exec,cpu,mmu,int,guest_errors,unimp > debug.log 2>&1