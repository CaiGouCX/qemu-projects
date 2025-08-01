#ifndef NVIC_H
#define NVIC_H

#include <stdint.h>

/* 中断号定义 */
#define NVIC_IRQ_SYSCTRL  2
#define NVIC_IRQ_UART0    5
#define NVIC_IRQ_SSI0     7
#define NVIC_IRQ_I2C0     8
#define NVIC_IRQ_PWM0     10
#define NVIC_IRQ_ADC0     14
#define NVIC_IRQ_WDT0     18

/* 初始化 NVIC */
void nvic_init(void);

/* 使能中断 */
void nvic_enable_irq(uint32_t irq);

/* 禁用中断 */
void nvic_disable_irq(uint32_t irq);

/* 设置中断优先级（0-7） */
void nvic_set_priority(uint32_t irq, uint32_t priority);

/* 清除挂起中断 */
void nvic_clear_pending(uint32_t irq);

/* 注册中断处理回调（占位，QEMU 无效） */
void nvic_register_handler(void (*handler)(void));

/* NVIC 中断处理函数（占位，QEMU 无效） */
void NVIC_Handler(void);

#endif /* NVIC_H */