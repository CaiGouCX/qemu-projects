#ifndef COMP_H
#define COMP_H

#include <stdint.h>

/* 初始化 Comparator0（设置参考电压 0-3.3V） */
void comp_init(float ref_voltage);

/* 获取比较器状态 */
uint32_t comp_get_status(void);

/* 注册中断处理回调（QEMU 无效） */
void comp_register_handler(void (*handler)(void));

/* Comparator 中断处理函数（QEMU 无效） */
void COMP0_Handler(void);

#endif /* COMP_H */