#include <stdint.h>

/* ========================== 寄存器定义 ========================== */
#define SYSCTL_BASE       0x400FE000
#define SYSCTL_RCGC1      (*((volatile uint32_t *)(SYSCTL_BASE + 0x104)))
#define SYSCTL_RCGC2      (*((volatile uint32_t *)(SYSCTL_BASE + 0x108)))

#define GPIOA_BASE        0x40004000
#define GPIOA_DIR         (*((volatile uint32_t *)(GPIOA_BASE + 0x400)))
#define GPIOA_AFSEL       (*((volatile uint32_t *)(GPIOA_BASE + 0x420)))
#define GPIOA_DEN         (*((volatile uint32_t *)(GPIOA_BASE + 0x51C)))
#define GPIOA_PUR         (*((volatile uint32_t *)(GPIOA_BASE + 0x510)))

#define UART0_BASE        0x4000C000
#define UART0_DR          (*((volatile uint32_t *)(UART0_BASE + 0x000)))
#define UART0_FR          (*((volatile uint32_t *)(UART0_BASE + 0x018)))
#define UART0_IBRD        (*((volatile uint32_t *)(UART0_BASE + 0x024)))
#define UART0_FBRD        (*((volatile uint32_t *)(UART0_BASE + 0x028)))
#define UART0_LCRH        (*((volatile uint32_t *)(UART0_BASE + 0x02C)))
#define UART0_CTL         (*((volatile uint32_t *)(UART0_BASE + 0x030)))
#define UART0_IM          (*((volatile uint32_t *)(UART0_BASE + 0x038)))
#define UART0_ICR         (*((volatile uint32_t *)(UART0_BASE + 0x044)))

#define NVIC_EN0          (*((volatile uint32_t *)(0xE000E100)))

/* ========================== 中断处理函数指针 ========================== */
static void (*uart0_handler)(void) = 0;

/* ========================== UART 初始化 ========================== */
void uart_init(uint32_t baud) {
    /* [QEMU 无效] 使能 UART0 和 GPIOA 模块时钟 */
    SYSCTL_RCGC1 |= 0x00000001;  // UART0 模块
    SYSCTL_RCGC2 |= 0x00000001;  // GPIOA 模块

    /* [QEMU 无效] 等待时钟稳定 */
    for (volatile int i = 0; i < 1000; i++);

    /* [QEMU 无效] 配置 PA0 和 PA1 为 UART 功能 */
    GPIOA_DIR &= ~0x03;      // PA0, PA1 设为输入（UART RX/TX）
    GPIOA_AFSEL |= 0x03;     // 启用复用功能
    GPIOA_PUR |= 0x03;       // 启用上拉
    GPIOA_DEN |= 0x03;       // 启用数字功能

    /* 配置 UART0 */
    UART0_CTL &= ~0x01;      // 禁用 UART 以进行设置

    /* [QEMU 无效] 波特率设置 */
    UART0_IBRD = 8;          // 整数部分: 16MHz / (16 * 115200) ≈ 8.68
    UART0_FBRD = 44;         // 小数部分: 0.68 * 64 + 0.5 ≈ 44

    /* [QEMU 有效] 设置数据格式（8位数据，无校验，1停止位） */
    UART0_LCRH = 0x60;

    /* [QEMU 有效] 启用 UART */
    UART0_CTL |= 0x01;
}

/* ========================== 发送单个字符 ========================== */
void uart_send_char(char c) {
    while (UART0_FR & 0x20);  // 等待 TX FIFO 非满（位5=1表示满）
    UART0_DR = c;
}

/* ========================== 发送字符串 ========================== */
void uart_send_string(const char *str) {
    while (*str) {
        uart_send_char(*str++);
    }
}

/* ========================== 注册中断处理函数（QEMU 无效） ========================== */
void uart_register_handler(void (*handler)(void)) {
    uart0_handler = handler;

    /* [QEMU 无效] 使能接收中断 */
    UART0_IM |= 0x10;        // 使能 UART 接收中断
    NVIC_EN0 |= (1 << 5);    // 启用 NVIC 中 UART0 中断（IRQ#5）
}

/* ========================== UART0 中断处理函数（QEMU 无效） ========================== */
void UART0_Handler(void) {
    if (uart0_handler) {
        uart0_handler();
    }

    /* [QEMU 无效] 清除接收中断标志 */
    UART0_ICR = 0x10;
}
