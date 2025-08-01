#include "ssi.h"

/* ========================== 寄存器定义 ========================== */
#define SYSCTL_BASE         0x400FE000
#define SYSCTL_RCGCSSI      (*((volatile uint32_t *)(SYSCTL_BASE + 0x61C)))

#define SSI0_BASE           0x40008000
#define SSI0_CR0            (*((volatile uint32_t *)(SSI0_BASE + 0x000)))
#define SSI0_CR1            (*((volatile uint32_t *)(SSI0_BASE + 0x004)))
#define SSI0_DR             (*((volatile uint32_t *)(SSI0_BASE + 0x008)))
#define SSI0_CPSR           (*((volatile uint32_t *)(SSI0_BASE + 0x010)))
#define SSI0_IM             (*((volatile uint32_t *)(SSI0_BASE + 0x014)))
#define SSI0_MIS            (*((volatile uint32_t *)(SSI0_BASE + 0x01C)))
#define SSI0_ICR            (*((volatile uint32_t *)(SSI0_BASE + 0x020)))

#define NVIC_EN0            (*((volatile uint32_t *)(0xE000E100)))

/* ========================== 中断处理函数指针 ========================== */
static void (*ssi0_handler)(void) = 0;

/* ========================== SSI 初始化 ========================== */
void ssi_init(uint32_t speed) {
    /* [QEMU 无效] 使能 SSI0 模块时钟 */
    SYSCTL_RCGCSSI |= 0x00000001;

    /* [QEMU 无效] 等待时钟稳定 */
    for (volatile int i = 0; i < 1000; i++);

    /* [QEMU 有效] 配置 SSI0 为 SPI 主站模式 */
    SSI0_CR1 &= ~0x00000002;      /* 禁用 SSI */
    SSI0_CR0 = 0x00000007;        /* 8 位数据，Freescale SPI 模式，极性/相位=0 */
    SSI0_CPSR = (16000000 / speed) & ~0x1; /* 时钟分频，16MHz 系统时钟 */
    SSI0_CR1 |= 0x00000002;       /* 启用 SSI 主站模式 */
}

/* ========================== 向从设备写入数据 ========================== */
void ssi_write(uint8_t *data, uint32_t len) {
    /* [QEMU 有效] 发送数据 */
    for (uint32_t i = 0; i < len; i++) {
        SSI0_DR = data[i];        /* 写入数据 */
        while (SSI0_MIS & 0x00000004); /* 等待传输完成（TX FIFO 空） */
    }
}

/* ========================== 注册中断处理函数（QEMU 无效） ========================== */
void ssi_register_handler(void (*handler)(void)) {
    ssi0_handler = handler;

    /* [QEMU 无效] 使能 SSI0 中断 */
    SSI0_IM |= 0x00000008;        /* 使能 TX 中断 */

    /* [QEMU 无效] 启用 NVIC 中 SSI0 中断（IRQ#7） */
    NVIC_EN0 |= (1 << 7);
}

/* ========================== SSI0 中断处理函数（QEMU 无效） ========================== */
void SSI0_Handler(void) {
    if (ssi0_handler) {
        ssi0_handler();
    }

    /* [QEMU 无效] 清除中断标志 */
    SSI0_ICR = 0x00000008;
}