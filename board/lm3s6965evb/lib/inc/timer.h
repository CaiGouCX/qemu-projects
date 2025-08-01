#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/* 初始化 Timer0A，设置周期（单位：系统时钟周期） */
void timer_init(uint32_t period);

/* 启动 Timer0A */
void timer_start(void);

/* 停止 Timer0A */
void timer_stop(void);

/* 注册中断处理回调 */
void timer_register_handler(void (*handler)(void));

/* ========================== Timer0A 中断处理函数（QEMU 无效） ========================== */
void TMR0A_Handler(void); 

#endif /* TIMER_H */