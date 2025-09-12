# 工具链
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# 路径定义
RTT_DIR = $(CURDIR)/rt-thread
PROJECT_DIR = /home/chengxin/qemu-projects
BOARD_DIR = $(PROJECT_DIR)/board/lm3s6965evb
SRC_DIR = $(PROJECT_DIR)/lm3s6965evb
LIB_SRC_DIR = $(BOARD_DIR)/lib/src
LIBSRC_DIR = $(PROJECT_DIR)/lm3s6965evb/rt-thread/bsp/lm3s6965evb/Libraries/driverlib
LIB_INC_DIR = $(BOARD_DIR)/lib/inc
BSP_DIR = $(RTT_DIR)/bsp/lm3s6965evb
BUILD_DIR = $(PROJECT_DIR)/build/lm3s6965evb
LINKER_SCRIPT = $(BOARD_DIR)/linker_lm3s6965evb.ld

# 编译选项
CFLAGS = -O2 -mcpu=cortex-m3 -mthumb -ffreestanding -g \
         -mfloat-abi=soft \
         -I$(BSP_DIR)/Libraries \
         -I$(BSP_DIR)/Libraries/inc \
         -I$(BSP_DIR)/Libraries/driverlib \
         -I$(BSP_DIR) \
         -I$(RTT_DIR)/include \
         -I$(RTT_DIR)/libcpu/arm/cortex-m3 \
         -I$(SRC_DIR) \
         -I$(LIB_INC_DIR) \
         -I$(RTT_DIR)/components/finsh \
         -I$(RTT_DIR)/components/drivers/include \
         -specs=nosys.specs

# 汇编选项
ASFLAGS = -mcpu=cortex-m3 -mthumb

# 链接选项
LDFLAGS = -T $(LINKER_SCRIPT) -mcpu=cortex-m3 -mthumb -Wl,--gc-sections -specs=nosys.specs

# 源文件
# RT-Thread 核心源文件
RTT_SRC = \
  $(RTT_DIR)/src/clock.c \
  $(RTT_DIR)/src/cpu_up.c \
  $(RTT_DIR)/src/components.c \
  $(RTT_DIR)/src/defunct.c \
  $(RTT_DIR)/src/idle.c \
  $(RTT_DIR)/src/ipc.c \
  $(RTT_DIR)/src/irq.c \
  $(RTT_DIR)/src/kservice.c \
  $(RTT_DIR)/src/mem.c \
  $(RTT_DIR)/src/memheap.c \
  $(RTT_DIR)/src/mempool.c \
  $(RTT_DIR)/src/object.c \
  $(RTT_DIR)/src/scheduler_comm.c \
  $(RTT_DIR)/src/scheduler_up.c \
  $(RTT_DIR)/src/signal.c \
  $(RTT_DIR)/src/slab.c \
  $(RTT_DIR)/src/thread.c \
  $(RTT_DIR)/src/timer.c \
  $(RTT_DIR)/components/drivers/core/device.c \
  $(RTT_DIR)/bsp/lm3s6965evb/applications/startup.c \
  $(RTT_DIR)/bsp/lm3s6965evb/applications/application.c \
  $(RTT_DIR)/bsp/lm3s6965evb/drivers/serial.c \
  $(RTT_DIR)/bsp/lm3s6965evb/drivers/board.c


RTT_SRC += $(RTT_DIR)/src/klibc/kstring.c \
           $(RTT_DIR)/src/klibc/kstdio.c \
           $(RTT_DIR)/src/klibc/kerrno.c \
           $(RTT_DIR)/src/klibc/rt_vsscanf.c \
           $(RTT_DIR)/src/klibc/rt_vsnprintf_tiny.c

# CPU 相关源码
RTT_CPU_C = $(wildcard $(RTT_DIR)/libcpu/arm/cortex-m3/*.c)
RTT_CPU_S = $(RTT_DIR)/libcpu/arm/cortex-m3/context_gcc.S
RTT_CPU = $(RTT_CPU_C) $(RTT_CPU_S)

# FinSH 相关源码
RTT_FINSH = $(wildcard $(RTT_DIR)/components/finsh/*.c)


# 裸机驱动源码
BARE_SRC = \
  $(LIBSRC_DIR)/pwm.c \
  $(LIBSRC_DIR)/qei.c \
  $(LIBSRC_DIR)/ssi.c \
  $(LIBSRC_DIR)/sysctl.c \
  $(LIBSRC_DIR)/systick.c \
  $(LIBSRC_DIR)/cpu.c \
  $(LIBSRC_DIR)/interrupt.c \
  $(LIBSRC_DIR)/gpio.c \
  $(LIBSRC_DIR)/uart.c

# 应用源码
APP_SRC = \
  $(SRC_DIR)/system_comm.c \
  $(SRC_DIR)/motor_main.c \
  $(SRC_DIR)/lm3s6965evb.c \
  $(SRC_DIR)/foc.c \
  $(SRC_DIR)/pid.c \
  $(SRC_DIR)/pwm_isr.c \
  $(SRC_DIR)/timer_isr.c \
  $(SRC_DIR)/globals.c

# 所有源文件
SRCS = $(RTT_SRC) \
       $(RTT_CPU) \
       $(RTT_FINSH) \
       $(BARE_SRC) \
       $(APP_SRC) \
       $(BOARD_DIR)/startup_lm3s6965evb.s

# 目标文件（保留路径结构）
OBJS = $(patsubst $(PROJECT_DIR)/%.c,$(BUILD_DIR)/%.o,$(filter %.c,$(SRCS))) \
       $(patsubst $(PROJECT_DIR)/%.s,$(BUILD_DIR)/%.o,$(filter %.s,$(SRCS))) \
       $(patsubst $(PROJECT_DIR)/%.S,$(BUILD_DIR)/%.o,$(filter %.S,$(SRCS)))

# 默认目标
all: $(BUILD_DIR) $(BUILD_DIR)/motor_control.bin

# 创建构建目录
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# 编译 C 文件
$(BUILD_DIR)/%.o: $(PROJECT_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# 编译汇编文件（大写 .S，预处理）
$(BUILD_DIR)/%.o: $(PROJECT_DIR)/%.S | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# 编译汇编文件（小写 .s，无预处理）
$(BUILD_DIR)/%.o: $(PROJECT_DIR)/%.s | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# 链接生成 ELF 并输出 .map 文件
$(BUILD_DIR)/motor_control.elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -Wl,-Map=$(BUILD_DIR)/motor_control.map -o $@
	$(SIZE) $@

# 生成二进制文件
$(BUILD_DIR)/motor_control.bin: $(BUILD_DIR)/motor_control.elf
	$(OBJCOPY) -O binary --gap-fill 0xff $< $@

# 清理
clean:
	rm -rf $(BUILD_DIR)

# 调试目标：打印变量
print-%:
	@echo $* = $($*)

.PHONY: all clean print-%