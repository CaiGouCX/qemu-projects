#!/bin/bash
# 最小化 QEMU 启动脚本 for ARM Cortex-M4

# 捕获 Ctrl+C 清理 QEMU 进程
trap 'echo "Terminating QEMU..."; killall -9 qemu-system-arm 2>/dev/null; exit 1' SIGINT SIGTERM

# 创建 build 目录
mkdir -p /home/chengxin/qemu-projects/build

# 清除旧文件
rm -f /home/chengxin/qemu-projects/build/hello_m4.*

echo "Compiling bare-metal program for ARM Cortex-M4..."
# 编译
arm-none-eabi-gcc -c -O2 -mcpu=cortex-m4 -mthumb -mno-relax -ffreestanding -nostdlib \
    /home/chengxin/qemu-projects/hello.c -o /home/chengxin/qemu-projects/build/hello_m4.o

# 链接
arm-none-eabi-ld -Ttext=0x00000000 \
    /home/chengxin/qemu-projects/build/hello_m4.o -o /home/chengxin/qemu-projects/build/hello_m4.elf

echo "Starting QEMU for ARM Cortex-M4..."
qemu-system-arm -nographic -M lm3s6965evb -cpu cortex-m4 \
    -kernel /home/chengxin/qemu-projects/build/hello_m4.elf \
    -serial mon:stdio | tee /home/chengxin/qemu-projects/build/output_m4.txt