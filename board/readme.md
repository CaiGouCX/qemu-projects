# Readme

## 测试程序hello.c
## 架构rsicv M3 M4任选执行对应脚本

# ✅ 各主流架构对应的 QEMU 模拟开发板型号（适合裸机）
## 架构	QEMU 机器名 -M	推荐型号 / 板子	描述 / 备注
### ARMv7-M	lm3s6965evb	Luminary LM3S6965 EVB	QEMU 官方支持的 Cortex-M3 板，适合裸机程序仿真（定时器、中断、串口）
### ARMv7-A	vexpress-a9	Versatile Express A9	双核 Cortex-A9，支持 Linux/裸机，外设全
### ARMv8-A	virt	虚拟通用板（64位）	官方推荐 64 位 ARM 板，Linux/裸机皆可
### MIPS	malta	MIPS Malta	工业级开发板，支持 YAMON、裸机、Linux，串口完善
### RISC-V	virt	riscv64-virt（QEMU 默认）	RISC-V 官方通用开发板，可跑裸机、Linux
### x86	默认（无需 -M）	i386 PC / q35 / isapc	QEMU 默认平台（BIOS + 可替换 UEFI），适合学习启动加载、UEFI、裸机开发
### PowerPC	mac99 或 prep	PowerMac G3 / PReP	可运行 MacOS 9 / Linux，适合研究旧架构
### SuperH	shix	SH7750R (SH4)	支持裸机程序仿真，串口、内存清晰
### SPARC	sun4m 或 leon3	SPARCstation / Leon3	用于运行 Solaris/Linux，也可裸机测试
### Xtensa	lx60	Tensilica Xtensa LX60	用于语音/信号处理类芯片，ESP32 同源
### MicroBlaze	petalogix-ml605	Xilinx ML605	FPGA SoC 架构，适合学习软核、裸机仿真
### OpenRISC	or1k-sim	OpenRISC 1200	自由架构，裸机开发简单，QEMU 支持良好

# ✅ 推荐最小启动配置示例（以裸机为目标）

---
### MIPS Malta ：
### qemu-system-mips -M malta -kernel hello.elf -nographic


### ARM Cortex-M3：
### qemu-system-arm -M lm3s6965evb -kernel hello.elf -nographic


### RISC-V：
### qemu-system-riscv64 -M virt -bios none -kernel hello.elf -nographic

---

### arm-none-eabi-nm /home/chengxin/qemu-projects/build/lm3s6965evb/hello_lm3s6965evb.elf
### arm-none-eabi-objdump -d /home/chengxin/qemu-projects/build/lm3s6965evb/hello_lm3s6965evb.elf -j .isr_vector

### arm-none-eabi-readelf -S /home/chengxin/qemu-projects/build/lm3s6965evb/hello_lm3s6965evb.elf
### arm-none-eabi-objcopy -O binary --gap-fill 0xff /home/chengxin/qemu-projects/build/lm3s6965evb/hello_lm3s6965evb.elf /home/chengxin/qemu-projects/build/lm3s6965evb/hello_lm3s6965evb.bin