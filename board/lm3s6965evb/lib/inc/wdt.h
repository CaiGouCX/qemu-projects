#ifndef WDT_H
#define WDT_H

#include <stdint.h>

/* 初始化 WDT0，设置超时周期（单位：毫秒，例如 1000） */
void wdt_init(uint32_t timeout_ms);

/* 喂狗（清除计数器） */
void wdt_feed(void);

/* 注册中断处理回调（QEMU 无效） */
void wdt_register_handler(void (*handler)(void));

/* WDT0 中断处理函数（QEMU 无效） */
void WATCHDOG0_Handler(void);

#endif /* WDT_H */