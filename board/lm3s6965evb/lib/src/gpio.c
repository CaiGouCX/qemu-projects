#include "gpio.h"

/* ========================== 寄存器定义 ========================== */
#define SYSCTL_BASE         0x400FE000
#define SYSCTL_RCGCGPIO     (*((volatile uint32_t *)(SYSCTL_BASE + 0x608)))

#define GPIOA_BASE          0x40004000
#define GPIOA_DIR           (*((volatile uint32_t *)(GPIOA_BASE + 0x400)))
#define GPIOA_AFSEL         (*((volatile uint32_t *)(GPIOA_BASE + 0x420)))
#define GPIOA_DEN           (*((volatile uint32_t *)(GPIOA_BASE + 0x51C)))
#define GPIOA_PUR           (*((volatile uint32_t *)(GPIOA_BASE + 0x510)))
#define GPIOA_DATA(pin)     (*((volatile uint32_t *)(GPIOA_BASE + (pin << 2))))
#define GPIOA_IM            (*((volatile uint32_t *)(GPIOA_BASE + 0x410)))
#define GPIOA_MIS           (*((volatile uint32_t *)(GPIOA_BASE + 0x418)))
#define GPIOA_ICR           (*((volatile uint32_t *)(GPIOA_BASE + 0x41C)))

#define NVIC_EN0            (*((volatile uint32_t *)(0xE000E100)))

/* ========================== 中断处理函数指针 ========================== */
static void (*gpioa_handler)(void) = 0;

/* ========================== GPIO 初始化 ========================== */
void gpio_init(uint32_t port, uint32_t pin, uint32_t mode) {
    if (port != GPIO_PORT_A) return; /* 仅支持 GPIOA */

    /* [QEMU 无效] 使能 GPIOA 模块时钟 */
    SYSCTL_RCGCGPIO |= 0x00000001;

    /* [QEMU 无效] 等待时钟稳定 */
    for (volatile int i = 0; i < 1000; i++);

    /* [QEMU 有效] 配置 GPIO 引脚 */
    uint32_t pin_mask = (1 << pin);
    if (mode == GPIO_MODE_INPUT) {
        GPIOA_DIR &= ~pin_mask;    /* 设置为输入 */
        GPIOA_PUR |= pin_mask;     /* 启用上拉 */
    } else if (mode == GPIO_MODE_OUTPUT) {
        GPIOA_DIR |= pin_mask;     /* 设置为输出 */
    } else if (mode == GPIO_MODE_AF) {
        GPIOA_AFSEL |= pin_mask;   /* 启用复用功能 */
    }
    GPIOA_DEN |= pin_mask;         /* 启用数字功能 */
}

/* ========================== 写 GPIO 引脚 ========================== */
void gpio_write(uint32_t port, uint32_t pin, uint32_t value) {
    if (port != GPIO_PORT_A) return;

    /* [QEMU 有效] 写入引脚值 */
    uint32_t pin_mask = (1 << pin);
    GPIOA_DATA(pin_mask) = value ? pin_mask : 0;
}

/* ========================== 读 GPIO 引脚 ========================== */
uint32_t gpio_read(uint32_t port, uint32_t pin) {
    if (port != GPIO_PORT_A) return 0;

    /* [QEMU 有效] 读取引脚值 */
    uint32_t pin_mask = (1 << pin);
    return (GPIOA_DATA(pin_mask) & pin_mask) ? 1 : 0;
}

/* ========================== 注册中断处理函数（QEMU 无效） ========================== */
void gpio_register_handler(uint32_t port, uint32_t pin, void (*handler)(void)) {
    if (port != GPIO_PORT_A) return;

    gpioa_handler = handler;

    /* [QEMU 无效] 使能指定引脚中断 */
    uint32_t pin_mask = (1 << pin);
    GPIOA_IM |= pin_mask;

    /* [QEMU 无效] 启用 NVIC 中 GPIOA 中断（IRQ#0） */
    NVIC_EN0 |= (1 << 0);
}

/* ========================== GPIOA 中断处理函数（QEMU 无效） ========================== */
void GPIOA_Handler(void) {
    if (gpioa_handler) {
        gpioa_handler();
    }

    /* [QEMU 无效] 清除中断标志 */
    GPIOA_ICR = GPIOA_MIS;
}