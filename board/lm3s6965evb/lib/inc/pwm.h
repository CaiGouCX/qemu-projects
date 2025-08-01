#ifndef PWM_H
#define PWM_H

#include <stdint.h>

/* 初始化 PWM0 生成器 0，设置频率（单位：Hz，例如 1000） */
void pwm_init(uint32_t freq);

/* 设置 PWM0 生成器 0 的占空比（0-100%） */
void pwm_set_duty(uint32_t duty);

/* 注册中断处理回调（QEMU 无效） */
void pwm_register_handler(void (*handler)(void));

/* PWM0 中断处理函数（QEMU 无效） */
void PWM0_Handler(void);

#endif /* PWM_H */