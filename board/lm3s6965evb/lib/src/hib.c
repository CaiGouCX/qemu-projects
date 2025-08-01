#include "hib.h"

/* ========================== 寄存器定义 ========================== */
#define SYSCTL_BASE         0x400FE000
#define SYSCTL_RCGCHIB      (*((volatile uint32_t *)(SYSCTL_BASE + 0x614)))

#define HIB_BASE            0x400FC000
#define HIB_RTCC            (*((volatile uint32_t *)(HIB_BASE + 0x000)))
#define HIB_CTL             (*((volatile uint32_t *)(HIB_BASE + 0x010)))
#define HIB_IM              (*((volatile uint32_t *)(HIB_BASE + 0x014)))
#define HIB_RIS             (*((volatile uint32_t *)(HIB_BASE + 0x018)))
#define HIB_MIS             (*((volatile uint32_t *)(HIB_BASE + 0x01C)))
#define HIB_IC              (*((volatile uint32_t *)(HIB_BASE + 0x020)))

#define NVIC_EN0            (*((volatile uint32_t *)(0xE000E100)))

/* ========================== 中断处理函数指针 ========================== */
static void (*hib_handler)(void) = 0;

/* ========================== HIB 初始化 ========================== */
void hib_init(void) {
    /* [QEMU 无效] 使能 HIB 模块时钟 */
    SYSCTL_RCGCHIB |= 0x00000001;

    /* [QEMU 无效] 等待时钟稳定 */
    for (volatile int i = 0; i < 1000; i++);

    /* [QEMU 有效] 启用 RTC */
    HIB_CTL |= 0x00000001; /* 启用 RTC */
}

/* ========================== 设置 RTC 时间 ========================== */
void hib_set_rtc(uint32_t seconds) {
    /* [QEMU 有效] 设置 RTC 计数 */
    HIB_RTCC = seconds;
}

/* ========================== 获取 RTC 时间 ========================== */
uint32_t hib_get_rtc(void) {
    /* [QEMU 有效] 返回 RTC 计数 */
    return HIB_RTCC;
}

/* ========================== 进入休眠模式（QEMU 无效） ========================== */
void hib_enter_hibernation(void) {
    /* [QEMU 无效] 触发休眠 */
    HIB_CTL |= 0x00000004; /* 进入休眠模式 */
}

/* ========================== 注册中断处理函数（QEMU 无效） ========================== */
void hib_register_handler(void (*handler)(void)) {
    hib_handler = handler;

    /* [QEMU 无效] 使能 HIB 中断 */
    HIB_IM |= 0x00000001; /* RTC 匹配中断 */

    /* [QEMU 无效] 启用 NVIC 中 HIB 中断（IRQ#31） */
    NVIC_EN0 |= (1 << 31);
}

/* ========================== HIB 中断处理函数（QEMU 无效） ========================== */
void HIB_Handler(void) {
    if (hib_handler) {
        hib_handler();
    }

    /* [QEMU 无效] 清除中断标志 */
    HIB_IC = 0x00000001;
}