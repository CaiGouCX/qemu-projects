#include "sysctrl.h"

/* ========================== 寄存器定义 ========================== */
#define SYSCTL_BASE         0x400FE000
#define SYSCTL_RCGC0        (*((volatile uint32_t *)(SYSCTL_BASE + 0x100)))
#define SYSCTL_RCGC1        (*((volatile uint32_t *)(SYSCTL_BASE + 0x104)))
#define SYSCTL_RCGC2        (*((volatile uint32_t *)(SYSCTL_BASE + 0x108)))
#define SYSCTL_SRCR0        (*((volatile uint32_t *)(SYSCTL_BASE + 0x040)))
#define SYSCTL_SRCR1        (*((volatile uint32_t *)(SYSCTL_BASE + 0x044)))
#define SYSCTL_SRCR2        (*((volatile uint32_t *)(SYSCTL_BASE + 0x048)))
#define SYSCTL_RCC          (*((volatile uint32_t *)(SYSCTL_BASE + 0x060)))
#define SYSCTL_RIS          (*((volatile uint32_t *)(SYSCTL_BASE + 0x050)))
#define SYSCTL_IMC          (*((volatile uint32_t *)(SYSCTL_BASE + 0x054)))
#define SYSCTL_MISC         (*((volatile uint32_t *)(SYSCTL_BASE + 0x058)))

#define NVIC_EN0            (*((volatile uint32_t *)(0xE000E100)))

/* ========================== 中断处理函数指针 ========================== */
static void (*sysctrl_handler)(void) = 0;

/* ========================== 系统时钟初始化 ========================== */
void sysctrl_init(void) {
    /* [QEMU 无效] 配置 16MHz 主振荡器（默认） */
    SYSCTL_RCC = 0x078E3AD1; /* 使用主振荡器，无 PLL，分频 1 */
}

/* ========================== 使能外设时钟 ========================== */
void sysctrl_enable_periph(uint32_t periph) {
    /* [QEMU 有效] 使能外设时钟 */
    switch (periph) {
        case SYSCTRL_PERIPH_UART0:
            SYSCTL_RCGC1 |= (1 << 0); /* UART0 */
            break;
        case SYSCTRL_PERIPH_SSI0:
            SYSCTL_RCGC1 |= (1 << 2); /* SSI0 */
            break;
        case SYSCTRL_PERIPH_I2C0:
            SYSCTL_RCGC1 |= (1 << 4); /* I2C0 */
            break;
        case SYSCTRL_PERIPH_PWM0:
            SYSCTL_RCGC0 |= (1 << 20); /* PWM0 */
            break;
        case SYSCTRL_PERIPH_ADC0:
            SYSCTL_RCGC0 |= (1 << 16); /* ADC0 */
            break;
        case SYSCTRL_PERIPH_GPIOA:
            SYSCTL_RCGC2 |= (1 << 0); /* GPIOA */
            break;
        default:
            return;
    }

    /* [QEMU 无效] 等待时钟稳定 */
    for (volatile int i = 0; i < 1000; i++);
}

/* ========================== 复位外设 ========================== */
void sysctrl_reset_periph(uint32_t periph) {
    /* [QEMU 有效] 复位外设 */
    switch (periph) {
        case SYSCTRL_PERIPH_UART0:
            SYSCTL_SRCR1 |= (1 << 0); /* UART0 */
            SYSCTL_SRCR1 &= ~(1 << 0);
            break;
        case SYSCTRL_PERIPH_SSI0:
            SYSCTL_SRCR1 |= (1 << 2); /* SSI0 */
            SYSCTL_SRCR1 &= ~(1 << 2);
            break;
        case SYSCTRL_PERIPH_I2C0:
            SYSCTL_SRCR1 |= (1 << 4); /* I2C0 */
            SYSCTL_SRCR1 &= ~(1 << 4);
            break;
        case SYSCTRL_PERIPH_PWM0:
            SYSCTL_SRCR0 |= (1 << 20); /* PWM0 */
            SYSCTL_SRCR0 &= ~(1 << 20);
            break;
        case SYSCTRL_PERIPH_ADC0:
            SYSCTL_SRCR0 |= (1 << 16); /* ADC0 */
            SYSCTL_SRCR0 &= ~(1 << 16);
            break;
        case SYSCTRL_PERIPH_GPIOA:
            SYSCTL_SRCR2 |= (1 << 0); /* GPIOA */
            SYSCTL_SRCR2 &= ~(1 << 0);
            break;
        default:
            return;
    }

    /* [QEMU 无效] 等待复位完成 */
    for (volatile int i = 0; i < 1000; i++);
}

/* ========================== 注册中断处理函数（QEMU 无效） ========================== */
void sysctrl_register_handler(void (*handler)(void)) {
    sysctrl_handler = handler;

    /* [QEMU 无效] 使能 SysCtrl 中断 */
    SYSCTL_IMC |= 0x00000010; /* PLL 锁定中断 */

    /* [QEMU 无效] 启用 NVIC 中 SysCtrl 中断（IRQ#2） */
    NVIC_EN0 |= (1 << 2);
}

/* ========================== SysCtrl 中断处理函数（QEMU 无效） ========================== */
void SYSCTL_Handler(void) {
    if (sysctrl_handler) {
        sysctrl_handler();
    }

    /* [QEMU 无效] 清除中断标志 */
    SYSCTL_MISC = 0x00000010;
}