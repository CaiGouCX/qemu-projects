#ifndef SSI_H
#define SSI_H

#include <stdint.h>

/* 初始化 SSI0，设置 SPI 速度（单位：bps，例如 1000000） */
void ssi_init(uint32_t speed);

/* 向从设备写入数据 */
void ssi_write(uint8_t *data, uint32_t len);

/* 注册中断处理回调（QEMU 无效） */
void ssi_register_handler(void (*handler)(void));

/* SSI0 中断处理函数（QEMU 无效） */
void SSI0_Handler(void);

#endif /* SSI_H */