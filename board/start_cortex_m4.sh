#!/bin/bash
# 最小化 QEMU 启动脚本 for ARM Cortex-M4

trap 'echo "Terminating QEMU..."; killall -9 qemu-system-arm 2>/dev/null; exit 1' SIGINT SIGTERM

mkdir -p /home/chengxin/qemu-projects/build/m4
rm -f /home/chengxin/qemu-projects/build/m4/*

if [ ! -f /home/chengxin/qemu-projects/board/hello.c ] || [ ! -f /home/chengxin/qemu-projects/board/startup_m4.s ] || [ ! -f /home/chengxin/qemu-projects/board/linker_m4.ld ]; then
    echo "Error: hello.c, startup_m4.s, or linker_m4.ld not found"
    exit 1
fi

echo "Compiling for ARM Cortex-M4..."
arm-none-eabi-gcc -c -O2 -mcpu=cortex-m4 -mthumb -ffreestanding -nostdlib \
    /home/chengxin/qemu-projects/board/hello.c -o /home/chengxin/qemu-projects/build/m4/hello_m4.o || exit 1
arm-none-eabi-gcc -c -O2 -mcpu=cortex-m4 -mthumb \
    /home/chengxin/qemu-projects/board/startup_m4.s -o /home/chengxin/qemu-projects/build/m4/startup_m4.o || exit 1

arm-none-eabi-ld -T /home/chengxin/qemu-projects/board/linker_m4.ld \
    /home/chengxin/qemu-projects/build/m4/startup_m4.o /home/chengxin/qemu-projects/build/m4/hello_m4.o \
    -o /home/chengxin/qemu-projects/build/m4/hello_m4.elf || exit 1

echo "Starting QEMU for ARM Cortex-M4..."
qemu-system-arm -nographic -M lm3s6965evb -cpu cortex-m4 \
    -kernel /home/chengxin/qemu-projects/build/m4/hello_m4.elf \
    -serial mon:stdio | tee /home/chengxin/qemu-projects/build/m4/output_m4.txt