#ifndef I2C_H
#define I2C_H

#include <stdint.h>

/* 初始化 I2C0，设置速度（单位：bps，例如 100000） */
void i2c_init(uint32_t speed);

/* 向从设备写入数据 */
void i2c_write(uint8_t addr, uint8_t *data, uint32_t len);

/* 注册中断处理回调（QEMU 无效） */
void i2c_register_handler(void (*handler)(void));

/* I2C0 中断处理函数（QEMU 无效） */
void I2C0_Handler(void);

#endif /* I2C_H */