#include "wdt.h"

/* ========================== 寄存器定义 ========================== */
#define SYSCTL_BASE         0x400FE000
#define SYSCTL_RCGCWD       (*((volatile uint32_t *)(SYSCTL_BASE + 0x600)))

#define WDT0_BASE           0x40000000
#define WDT0_LOAD           (*((volatile uint32_t *)(WDT0_BASE + 0x000)))
#define WDT0_CTL            (*((volatile uint32_t *)(WDT0_BASE + 0x008)))
#define WDT0_ICR            (*((volatile uint32_t *)(WDT0_BASE + 0x00C)))
#define WDT0_RIS            (*((volatile uint32_t *)(WDT0_BASE + 0x010)))
#define WDT0_MIS            (*((volatile uint32_t *)(WDT0_BASE + 0x018)))
#define WDT0_LOCK           (*((volatile uint32_t *)(WDT0_BASE + 0xC00)))

#define NVIC_EN0            (*((volatile uint32_t *)(0xE000E100)))

/* ========================== 中断处理函数指针 ========================== */
static void (*wdt0_handler)(void) = 0;

/* ========================== WDT 初始化 ========================== */
void wdt_init(uint32_t timeout_ms) {
    /* [QEMU 无效] 使能 WDT0 模块时钟 */
    SYSCTL_RCGCWD |= 0x00000001;

    /* [QEMU 无效] 等待时钟稳定 */
    for (volatile int i = 0; i < 1000; i++);

    /* [QEMU 有效] 配置 WDT0 */
    WDT0_LOCK = 0x1ACCE551;      /* 解锁寄存器 */
    WDT0_CTL = 0x00000000;        /* 禁用 WDT */
    WDT0_LOAD = (16000000 / 1000) * timeout_ms; /* 设置超时，16MHz 时钟 */
    WDT0_CTL = 0x00000003;        /* 启用 WDT 和复位 */
    WDT0_LOCK = 0x00000000;       /* 锁定寄存器 */
}

/* ========================== 喂狗 ========================== */
void wdt_feed(void) {
    /* [QEMU 有效] 清除计数器 */
    WDT0_ICR = 0xFFFFFFFF;
}

/* ========================== 注册中断处理函数（QEMU 无效） ========================== */
void wdt_register_handler(void (*handler)(void)) {
    wdt0_handler = handler;

    /* [QEMU 无效] 使能 WDT0 中断 */
    WDT0_CTL |= 0x00000001;       /* 启用中断（不触发复位） */

    /* [QEMU 无效] 启用 NVIC 中 WDT0 中断（IRQ#18） */
    NVIC_EN0 |= (1 << 18);
}

/* ========================== WDT0 中断处理函数（QEMU 无效） ========================== */
void WATCHDOG0_Handler(void) {
    if (wdt0_handler) {
        wdt0_handler();
    }

    /* [QEMU 无效] 清除中断标志 */
    WDT0_ICR = 0xFFFFFFFF;
}