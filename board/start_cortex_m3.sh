#!/bin/bash
# 最小化 QEMU 启动脚本 for ARM Cortex-M3

# 捕获 Ctrl+C 清理 QEMU 进程
trap 'echo "Terminating QEMU..."; killall -9 qemu-system-arm 2>/dev/null; exit 1' SIGINT SIGTERM

# 创建架构特定目录
mkdir -p /home/chengxin/qemu-projects/build/m3

# 清除旧文件
rm -f /home/chengxin/qemu-projects/build/m3/hello_m3.*

# 验证源文件
if [ ! -f /home/chengxin/qemu-projects/board/hello.c ]; then
    echo "Error: hello.c not found in /home/chengxin/qemu-projects/board"
    exit 1
fi

echo "Compiling bare-metal program for ARM Cortex-M3..."
# 编译
arm-none-eabi-gcc -c -O2 -mcpu=cortex-m3 -mthumb -ffreestanding -nostdlib \
    /home/chengxin/qemu-projects/board/hello.c -o /home/chengxin/qemu-projects/build/m3/hello_m3.o 2>&1 || {
    echo "Error: gcc compilation failed"
    exit 1
}

# 链接
arm-none-eabi-ld -Ttext=0x00000000 \
    /home/chengxin/qemu-projects/build/m3/hello_m3.o -o /home/chengxin/qemu-projects/build/m3/hello_m3.elf 2>&1 || {
    echo "Error: ld linking failed"
    exit 1
}

echo "Starting QEMU for ARM Cortex-M3..."
qemu-system-arm -nographic -M lm3s811evb -cpu cortex-m3 \
    -kernel /home/chengxin/qemu-projects/build/m3/hello_m3.elf \
    -serial mon:stdio | tee /home/chengxin/qemu-projects/build/m3/output_m3.txt