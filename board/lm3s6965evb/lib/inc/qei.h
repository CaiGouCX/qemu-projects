#ifndef QEI_H
#define QEI_H

#include <stdint.h>

/* 初始化 QEI0（设置最大位置） */
void qei_init(uint32_t max_pos);

/* 读取当前位置 */
uint32_t qei_get_position(void);

/* 读取速度 */
uint32_t qei_get_speed(void);

/* 注册中断处理回调（QEMU 无效） */
void qei_register_handler(void (*handler)(void));

/* QEI0 中断处理函数（QEMU 无效） */
void QEI0_Handler(void);

#endif /* QEI_H */