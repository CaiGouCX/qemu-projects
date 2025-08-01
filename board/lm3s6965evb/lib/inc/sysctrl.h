#ifndef SYSCTRL_H
#define SYSCTRL_H

#include <stdint.h>

/* 外设定义 */
#define SYSCTRL_PERIPH_UART0  0
#define SYSCTRL_PERIPH_SSI0   1
#define SYSCTRL_PERIPH_I2C0   2
#define SYSCTRL_PERIPH_PWM0   3
#define SYSCTRL_PERIPH_ADC0   4
#define SYSCTRL_PERIPH_GPIOA  5

/* 初始化系统时钟（默认 16MHz） */
void sysctrl_init(void);

/* 使能外设时钟 */
void sysctrl_enable_periph(uint32_t periph);

/* 复位外设 */
void sysctrl_reset_periph(uint32_t periph);

/* 注册中断处理回调（QEMU 无效） */
void sysctrl_register_handler(void (*handler)(void));

/* SysCtrl 中断处理函数（QEMU 无效） */
void SYSCTL_Handler(void);

#endif /* SYSCTRL_H */