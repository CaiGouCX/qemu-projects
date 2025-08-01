# Makefile: LM3S6965 项目，链接 liblm3s6965.a
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# 编译选项
CFLAGS = -O2 -mcpu=cortex-m3 -mthumb -ffreestanding -nostdlib -g -I/home/chengxin/qemu-projects/lib/lm3s6965
LDFLAGS = -T /home/chengxin/qemu-projects/board/lm3s6965evb/linker_lm3s6965evb.ld -L/home/chengxin/qemu-projects/lib/lm3s6965 -llm3s6965

# 源文件和目标
SRCS = /home/chengxin/qemu-projects/lm3s6965evb/hello_lm3s6965evb.c /home/chengxin/qemu-projects/board/lm3s6965evb/startup_lm3s6965evb.s
OBJDIR = /home/chengxin/qemu-projects/build/lm3s6965evb
OBJS = $(OBJDIR)/hello_lm3s6965evb.o $(OBJDIR)/startup_lm3s6965evb.o
TARGET = $(OBJDIR)/hello_lm3s6965evb.elf
BIN = $(OBJDIR)/hello_lm3s6965evb.bin

all: $(BIN)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/hello_lm3s6965evb.o: /home/chengxin/qemu-projects/lm3s6965evb/hello_lm3s6965evb.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/startup_lm3s6965evb.o: /home/chengxin/qemu-projects/board/lm3s6965evb/startup_lm3s6965evb.s | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@  # 显示内存使用

$(BIN): $(TARGET)
	$(OBJCOPY) -O binary --gap-fill 0xff $< $@

clean:
	rm -rf $(OBJDIR)

.PHONY: all clean