#include "timer.h"

/* ========================== 寄存器定义 ========================== */
#define SYSCTL_BASE         0x400FE000
#define SYSCTL_RCGCTIMER    (*((volatile uint32_t *)(SYSCTL_BASE + 0x604)))

#define TIMER0_BASE         0x40030000
#define TIMER0_CFG          (*((volatile uint32_t *)(TIMER0_BASE + 0x000)))
#define TIMER0_TAMR         (*((volatile uint32_t *)(TIMER0_BASE + 0x004)))
#define TIMER0_CTL          (*((volatile uint32_t *)(TIMER0_BASE + 0x00C)))
#define TIMER0_IMR          (*((volatile uint32_t *)(TIMER0_BASE + 0x018)))
#define TIMER0_MIS          (*((volatile uint32_t *)(TIMER0_BASE + 0x020)))
#define TIMER0_ICR          (*((volatile uint32_t *)(TIMER0_BASE + 0x024)))
#define TIMER0_TAILR        (*((volatile uint32_t *)(TIMER0_BASE + 0x028)))

#define NVIC_EN0            (*((volatile uint32_t *)(0xE000E100)))

/* ========================== 中断处理函数指针 ========================== */
static void (*timer0a_handler)(void) = 0;

/* ========================== Timer0A 初始化 ========================== */
void timer_init(uint32_t period) {
    /* [QEMU 无效] 使能 Timer0 模块时钟 */
    SYSCTL_RCGCTIMER |= 0x00000001;

    /* [QEMU 无效] 等待时钟稳定 */
    for (volatile int i = 0; i < 1000; i++);

    /* [QEMU 有效] 配置 Timer0A 为 32 位周期模式 */
    TIMER0_CTL &= ~0x00000001;    // 禁用 Timer0A
    TIMER0_CFG = 0x00000000;      // 32 位模式
    TIMER0_TAMR = 0x00000002;     // 周期模式
    TIMER0_TAILR = period - 1;    // 设置周期
}

/* ========================== 启动 Timer0A ========================== */
void timer_start(void) {
    /* [QEMU 有效] 启用 Timer0A */
    TIMER0_CTL |= 0x00000001;
}

/* ========================== 停止 Timer0A ========================== */
void timer_stop(void) {
    /* [QEMU 有效] 禁用 Timer0A */
    TIMER0_CTL &= ~0x00000001;
}

/* ========================== 注册中断处理函数（QEMU 无效） ========================== */
void timer_register_handler(void (*handler)(void)) {
    timer0a_handler = handler;

    /* [QEMU 无效] 使能 Timer0A 超时中断 */
    TIMER0_IMR |= 0x00000001;

    /* [QEMU 无效] 启用 NVIC 中 Timer0A 中断（IRQ#19） */
    NVIC_EN0 |= (1 << 19);
}

/* ========================== Timer0A 中断处理函数（QEMU 无效） ========================== */
void TMR0A_Handler(void) {
    if (timer0a_handler) {
        timer0a_handler();
    }

    /* [QEMU 无效] 清除超时中断标志 */
    TIMER0_ICR = 0x00000001;
}