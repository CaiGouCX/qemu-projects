#ifndef HIB_H
#define HIB_H

#include <stdint.h>

/* 初始化 HIB（启用 RTC） */
void hib_init(void);

/* 设置 RTC 时间（秒） */
void hib_set_rtc(uint32_t seconds);

/* 获取 RTC 时间（秒） */
uint32_t hib_get_rtc(void);

/* 进入休眠模式（QEMU 无效） */
void hib_enter_hibernation(void);

/* 注册中断处理回调（QEMU 无效） */
void hib_register_handler(void (*handler)(void));

/* HIB 中断处理函数（QEMU 无效） */
void HIB_Handler(void);

#endif /* HIB_H */