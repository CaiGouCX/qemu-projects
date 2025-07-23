#!/bin/bash
# 最小化 QEMU 启动脚本 for RISC-V 32-bit

# 捕获 Ctrl+C 清理 QEMU 进程
trap 'echo "Terminating QEMU..."; killall -9 qemu-system-riscv32 2>/dev/null; exit 1' SIGINT SIGTERM

# 创建架构特定目录
mkdir -p /home/chengxin/qemu-projects/build/riscv32

# 清除旧文件
rm -f /home/chengxin/qemu-projects/build/riscv32/hello_rv32.*

# 验证源文件
if [ ! -f /home/chengxin/qemu-projects/board/hello.c ]; then
    echo "Error: hello.c not found in /home/chengxin/qemu-projects/board"
    exit 1
fi

echo "Compiling bare-metal program for RISC-V 32-bit..."
# 编译
riscv64-unknown-elf-gcc -c -O2 -march=rv32im -mabi=ilp32 -mno-relax -ffreestanding -nostdlib \
    /home/chengxin/qemu-projects/board/hello.c -o /home/chengxin/qemu-projects/build/riscv32/hello_rv32.o 2>&1 || {
    echo "Error: gcc compilation failed"
    exit 1
}

# 链接
riscv64-unknown-elf-ld -m elf32lriscv -Ttext=0x80000000 \
    /home/chengxin/qemu-projects/build/riscv32/hello_rv32.o -o /home/chengxin/qemu-projects/build/riscv32/hello_rv32.elf 2>&1 || {
    echo "Error: ld linking failed"
    exit 1
}

echo "Starting QEMU for RISC-V 32-bit..."
qemu-system-riscv32 -nographic -M virt -cpu rv32 \
    -bios none -kernel /home/chengxin/qemu-projects/build/riscv32/hello_rv32.elf \
    -serial mon:stdio | tee /home/chengxin/qemu-projects/build/riscv32/output_rv32.txt