#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

/* GPIO 端口定义 */
#define GPIO_PORT_A 0

/* GPIO 模式定义 */
#define GPIO_MODE_INPUT  0
#define GPIO_MODE_OUTPUT 1
#define GPIO_MODE_AF     2 /* 复用功能（如 UART） */

/* 初始化 GPIO 引脚 */
void gpio_init(uint32_t port, uint32_t pin, uint32_t mode);

/* 写 GPIO 引脚 */
void gpio_write(uint32_t port, uint32_t pin, uint32_t value);

/* 读 GPIO 引脚 */
uint32_t gpio_read(uint32_t port, uint32_t pin);

/* 注册中断处理回调（QEMU 无效） */
void gpio_register_handler(uint32_t port, void (*handler)(void));


void GPIOA_Handler(void);

#endif /* GPIO_H */