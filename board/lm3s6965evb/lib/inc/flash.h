#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

/* 初始化 Flash 控制器 */
void flash_init(void);

/* 擦除 Flash 页面（地址必须 1KB 对齐） */
void flash_erase_page(uint32_t address);

/* 编程 Flash 字（地址必须 4 字节对齐） */
void flash_write_word(uint32_t address, uint32_t data);

/* 注册中断处理回调（QEMU 无效） */
void flash_register_handler(void (*handler)(void));

/* Flash 中断处理函数（QEMU 无效） */
void FLASH_Handler(void);

#endif /* FLASH_H */