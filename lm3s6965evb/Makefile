# 工具链
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# 路径变量
PROJECT_DIR = /home/chengxin/qemu-projects
BOARD_DIR = $(PROJECT_DIR)/board/lm3s6965evb
SRC_DIR = $(PROJECT_DIR)/lm3s6965evb
LIB_SRC_DIR = $(BOARD_DIR)/lib/src
LIB_INC_DIR = $(BOARD_DIR)/lib/inc
BUILD_DIR = $(PROJECT_DIR)/build/lm3s6965evb
LINKER_SCRIPT = $(BOARD_DIR)/linker_lm3s6965evb.ld

# 编译选项
CFLAGS = -O2 -mcpu=cortex-m3 -mthumb -ffreestanding -g \
         -I$(SRC_DIR) \
         -I$(LIB_INC_DIR) \
         -specs=nosys.specs

# 链接选项
LDFLAGS = -T $(LINKER_SCRIPT) -Wl,--gc-sections

# 源文件
SRCS = $(SRC_DIR)/system_comm.c \
       $(SRC_DIR)/motor_main.c \
       $(SRC_DIR)/lm3s6965evb.c \
       $(SRC_DIR)/globals.c \
       $(BOARD_DIR)/startup_lm3s6965evb.s \
       $(LIB_SRC_DIR)/pwm.c \
       $(LIB_SRC_DIR)/qei.c \
       $(LIB_SRC_DIR)/ssi.c \
       $(LIB_SRC_DIR)/sysctrl.c \
       $(LIB_SRC_DIR)/timer.c \
       $(LIB_SRC_DIR)/nvic.c

# 目标文件
# 提取文件名（不含路径）
SRC_C_NAMES := $(notdir $(filter %.c, $(SRCS)))
SRC_S_NAMES := $(notdir $(filter %.s, $(SRCS)))

# 目标对象文件
OBJS = $(addprefix $(BUILD_DIR)/, $(SRC_C_NAMES:.c=.o) $(SRC_S_NAMES:.s=.o))


# 默认目标
all: $(BUILD_DIR) $(BUILD_DIR)/motor_control.bin

# 创建构建目录
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# 汇编文件编译（支持通配）
$(BUILD_DIR)/%.o: $(BOARD_DIR)/%.s | $(BUILD_DIR)
	$(CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@

# 编译 SRC_DIR 下的 C 文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 编译 LIB_SRC_DIR 下的 C 文件
$(BUILD_DIR)/%.o: $(LIB_SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

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

.PHONY: all clean
