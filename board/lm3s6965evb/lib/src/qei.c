#include "qei.h"

/* ========================== 寄存器定义 ========================== */
#define SYSCTL_BASE         0x400FE000
#define SYSCTL_RCGCQEI      (*((volatile uint32_t *)(SYSCTL_BASE + 0x644)))

#define QEI0_BASE           0x4002C000
#define QEI0_CTL            (*((volatile uint32_t *)(QEI0_BASE + 0x000)))
#define QEI0_POS            (*((volatile uint32_t *)(QEI0_BASE + 0x004)))
#define QEI0_SPEED          (*((volatile uint32_t *)(QEI0_BASE + 0x008)))
#define QEI0_INTEN          (*((volatile uint32_t *)(QEI0_BASE + 0x010)))
#define QEI0_RIS            (*((volatile uint32_t *)(QEI0_BASE + 0x014)))
#define QEI0_ISC            (*((volatile uint32_t *)(QEI0_BASE + 0x018)))

#define NVIC_EN0            (*((volatile uint32_t *)(0xE000E100)))

/* ========================== 中断处理函数指针 ========================== */
static void (*qei0_handler)(void) = 0;

/* ========================== QEI 初始化 ========================== */
void qei_init(uint32_t max_pos) {
    /* [QEMU 无效] 使能 QEI0 模块时钟 */
    SYSCTL_RCGCQEI |= 0x00000001;

    /* [QEMU 无效] 等待时钟稳定 */
    for (volatile int i = 0; i < 1000; i++);

    /* [QEMU 有效] 配置 QEI0 */
    QEI0_CTL = 0x00000000;        /* 禁用 QEI */
    QEI0_POS = 0;                 /* 清零位置 */
    QEI0_CTL = 0x00000001 | (max_pos << 12); /* 启用 QEI，设置最大位置 */
}

/* ========================== 读取当前位置 ========================== */
uint32_t qei_get_position(void) {
    /* [QEMU 有效] 返回当前位置 */
    return QEI0_POS;
}

/* ========================== 读取速度 ========================== */
uint32_t qei_get_speed(void) {
    /* [QEMU 有效] 返回速度 */
    return QEI0_SPEED;
}

/* ========================== 注册中断处理函数（QEMU 无效） ========================== */
void qei_register_handler(void (*handler)(void)) {
    qei0_handler = handler;

    /* [QEMU 无效] 使能 QEI0 中断 */
    QEI0_INTEN |= 0x00000001;     /* 位置溢出中断 */

    /* [QEMU 无效] 启用 NVIC 中 QEI0 中断（IRQ#13） */
    NVIC_EN0 |= (1 << 13);
}

/* ========================== QEI0 中断处理函数（QEMU 无效） ========================== */
void QEI0_Handler(void) {
    if (qei0_handler) {
        qei0_handler();
    }

    /* [QEMU 无效] 清除中断标志 */
    QEI0_ISC = 0x00000001;
}