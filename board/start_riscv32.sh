#!/bin/bash
# 最小化 QEMU 启动脚本 for RISC-V 32-bit

trap 'echo "Terminating QEMU..."; killall -9 qemu-system-riscv32 2>/dev/null; exit 1' SIGINT SIGTERM

mkdir -p /home/chengxin/qemu-projects/build/riscv32
rm -f /home/chengxin/qemu-projects/build/riscv32/hello_rv32.*

if [ ! -f /home/chengxin/qemu-projects/board/hello.c ] || [ ! -f /home/chengxin/qemu-projects/board/linker_rv32.ld ]; then
    echo "Error: hello.c or linker_rv32.ld not found"
    exit 1
fi

echo "Compiling for RISC-V 32-bit..."
riscv64-unknown-elf-gcc -c -O2 -march=rv32im -mabi=ilp32 -mno-relax -ffreestanding -nostdlib \
    /home/chengxin/qemu-projects/board/hello.c -o /home/chengxin/qemu-projects/build/riscv32/hello_rv32.o || exit 1

riscv64-unknown-elf-ld -m elf32lriscv -T /home/chengxin/qemu-projects/board/linker_rv32.ld \
    /home/chengxin/qemu-projects/build/riscv32/hello_rv32.o -o /home/chengxin/qemu-projects/build/riscv32/hello_rv32.elf || exit 1

echo "Starting QEMU for RISC-V 32-bit..."
qemu-system-riscv32 -nographic -M virt -cpu rv32 \
    -bios none -kernel /home/chengxin/qemu-projects/build/riscv32/hello_rv32.elf \
    -serial mon:stdio | tee /home/chengxin/qemu-projects/build/riscv32/output_rv32.txt