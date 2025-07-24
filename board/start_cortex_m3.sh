#!/bin/bash
# 最小化 QEMU 启动脚本 for ARM Cortex-M3

trap 'echo "Terminating QEMU..."; killall -9 qemu-system-arm 2>/dev/null; exit 1' SIGINT SIGTERM

mkdir -p /home/chengxin/qemu-projects/build/m3
rm -f /home/chengxin/qemu-projects/build/m3/*

if [ ! -f /home/chengxin/qemu-projects/board/hello.c ] || [ ! -f /home/chengxin/qemu-projects/board/startup_m3.s ] || [ ! -f /home/chengxin/qemu-projects/board/linker_m3.ld ]; then
    echo "Error: hello.c, startup_m3.s, or linker_m3.ld not found"
    exit 1
fi

echo "Compiling for ARM Cortex-M3..."
arm-none-eabi-gcc -c -O2 -mcpu=cortex-m3 -mthumb -ffreestanding -nostdlib \
    /home/chengxin/qemu-projects/board/hello.c -o /home/chengxin/qemu-projects/build/m3/hello_m3.o || exit 1
arm-none-eabi-gcc -c -O2 -mcpu=cortex-m3 -mthumb \
    /home/chengxin/qemu-projects/board/startup_m3.s -o /home/chengxin/qemu-projects/build/m3/startup_m3.o || exit 1

arm-none-eabi-ld -T /home/chengxin/qemu-projects/board/linker_m3.ld \
    /home/chengxin/qemu-projects/build/m3/startup_m3.o /home/chengxin/qemu-projects/build/m3/hello_m3.o \
    -o /home/chengxin/qemu-projects/build/m3/hello_m3.elf || exit 1

echo "Starting QEMU for ARM Cortex-M3..."
qemu-system-arm -nographic -M lm3s811evb -cpu cortex-m3 \
    -kernel /home/chengxin/qemu-projects/build/m3/hello_m3.elf \
    -serial mon:stdio | tee /home/chengxin/qemu-projects/build/m3/output_m3.txt