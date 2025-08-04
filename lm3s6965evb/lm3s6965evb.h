/* lm3s6965evb.h: LM3S6965 硬件抽象定义 for motor control */
#ifndef LM3S6965EVB_H
#define LM3S6965EVB_H

#include <stdint.h>
#include "sysctrl.h"
#include "pwm.h"
#include "qei.h"
#include "timer.h"
#include "ssi.h"
#include "nvic.h"

/* 函数原型（从 sysctrl.h, pwm.h, qei.h, timer.h, ssi.h, nvic.h 提供） */
void SysCtrl_ClockSet(uint32_t freq); // 设置系统时钟 (Hz)
void PWM0_Init(uint32_t freq);        // 初始化 PWM0 (Hz)
void QEI0_Init(void);                 // 初始化 QEI0
uint32_t QEI0_GetPosition(void);      // 获取 QEI0 位置
void Timer0_Init(uint32_t freq);      // 初始化 Timer0 (Hz)
void Timer0_ClearInterrupt(void);     // 清除 Timer0 中断
void SSI0_Init(uint32_t freq);        // 初始化 SSI0 (SPI, Hz)
uint8_t SSI0_Receive(void);           // SPI 接收字节
void SSI0_Send(uint8_t data);         // SPI 发送字节
void PWM0_SetDuty(float duty);        // 设置 PWM0 占空比

#endif /* LM3S6965EVB_H */