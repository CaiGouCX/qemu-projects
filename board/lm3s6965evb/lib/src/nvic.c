#include "nvic.h"

/* ========================== 寄存器定义 ========================== */
#define NVIC_BASE           0xE000E000
#define NVIC_EN0            (*((volatile uint32_t *)(NVIC_BASE + 0x100)))
#define NVIC_DIS0           (*((volatile uint32_t *)(NVIC_BASE + 0x180)))
#define NVIC_PEND0          (*((volatile uint32_t *)(NVIC_BASE + 0x200)))
#define NVIC_UNPEND0        (*((volatile uint32_t *)(NVIC_BASE + 0x280)))
#define NVIC_PRI0           (*((volatile uint32_t *)(NVIC_BASE + 0x400)))
#define NVIC_PRI1           (*((volatile uint32_t *)(NVIC_BASE + 0x404)))
#define NVIC_PRI2           (*((volatile uint32_t *)(NVIC_BASE + 0x408)))
#define NVIC_PRI3           (*((volatile uint32_t *)(NVIC_BASE + 0x40C)))
#define NVIC_PRI4           (*((volatile uint32_t *)(NVIC_BASE + 0x410)))

/* ========================== 中断处理函数指针（占位） ========================== */
static void (*nvic_handler)(void) = 0;

/* ========================== NVIC 初始化 ========================== */
void nvic_init(void) {
    /* [QEMU 有效] 清除所有挂起中断 */
    NVIC_UNPEND0 = 0xFFFFFFFF;

    /* [QEMU 有效] 设置默认优先级为 0 */
    NVIC_PRI0 = 0;
    NVIC_PRI1 = 0;
    NVIC_PRI2 = 0;
    NVIC_PRI3 = 0;
    NVIC_PRI4 = 0;
}

/* ========================== 使能中断 ========================== */
void nvic_enable_irq(uint32_t irq) {
    if (irq > 34) return; /* LM3S6965 支持 IRQ 0-34 */

    /* [QEMU 有效] 使能指定中断 */
    NVIC_EN0 |= (1 << irq);
}

/* ========================== 禁用中断 ========================== */
void nvic_disable_irq(uint32_t irq) {
    if (irq > 34) return;

    /* [QEMU 有效] 禁用指定中断 */
    NVIC_DIS0 |= (1 << irq);
}

/* ========================== 设置中断优先级 ========================== */
void nvic_set_priority(uint32_t irq, uint32_t priority) {
    if (irq > 34 || priority > 7) return;

    /* [QEMU 有效] 设置优先级（高 3 位，0x00-0xE0） */
    uint32_t pri_reg = irq / 4; /* 每个 PRI 寄存器控制 4 个 IRQ */
    uint32_t shift = (irq % 4) * 8 + 5; /* 每 8 位，高 3 位有效 */
    volatile uint32_t *pri_ptr = &NVIC_PRI0 + pri_reg;
    *pri_ptr &= ~(0x7 << shift); /* 清除原优先级 */
    *pri_ptr |= (priority << shift); /* 设置新优先级 */
}

/* ========================== 清除挂起中断 ========================== */
void nvic_clear_pending(uint32_t irq) {
    if (irq > 34) return;

    /* [QEMU 有效] 清除挂起状态 */
    NVIC_UNPEND0 |= (1 << irq);
}

/* ========================== 注册中断处理函数（占位，QEMU 无效） ========================== */
void nvic_register_handler(void (*handler)(void)) {
    nvic_handler = handler;
    /* [QEMU 无效] NVIC 本身无专用中断 */
}

/* ========================== NVIC 中断处理函数（占位，QEMU 无效） ========================== */
void NVIC_Handler(void) {
    if (nvic_handler) {
        nvic_handler();
    }
    /* [QEMU 无效] NVIC 本身无中断 */
}