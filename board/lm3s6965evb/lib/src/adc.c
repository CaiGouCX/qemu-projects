#include "adc.h"

/* ========================== 寄存器定义 ========================== */
#define SYSCTL_BASE         0x400FE000
#define SYSCTL_RCGCADC      (*((volatile uint32_t *)(SYSCTL_BASE + 0x638)))

#define ADC0_BASE           0x40038000
#define ADC0_ACTSS          (*((volatile uint32_t *)(ADC0_BASE + 0x000)))
#define ADC0_EMUX           (*((volatile uint32_t *)(ADC0_BASE + 0x014)))
#define ADC0_PSSI           (*((volatile uint32_t *)(ADC0_BASE + 0x028)))
#define ADC0_SSMUX0         (*((volatile uint32_t *)(ADC0_BASE + 0x040)))
#define ADC0_SSCTL0         (*((volatile uint32_t *)(ADC0_BASE + 0x044)))
#define ADC0_SSFIFO0        (*((volatile uint32_t *)(ADC0_BASE + 0x048)))
#define ADC0_IM             (*((volatile uint32_t *)(ADC0_BASE + 0x008)))
#define ADC0_MIS            (*((volatile uint32_t *)(ADC0_BASE + 0x010)))
#define ADC0_ICR            (*((volatile uint32_t *)(ADC0_BASE + 0x018)))

#define NVIC_EN0            (*((volatile uint32_t *)(0xE000E100)))

/* ========================== 中断处理函数指针 ========================== */
static void (*adc0_handler)(void) = 0;

/* ========================== ADC 初始化 ========================== */
void adc_init(uint32_t channel) {
    if (channel > ADC_CHANNEL_3) return; /* 仅支持通道 0-3 */

    /* [QEMU 无效] 使能 ADC0 模块时钟 */
    SYSCTL_RCGCADC |= 0x00000001;

    /* [QEMU 无效] 等待时钟稳定 */
    for (volatile int i = 0; i < 1000; i++);

    /* [QEMU 有效] 配置 ADC0 采样器 SS0 */
    ADC0_ACTSS &= ~0x00000001;    /* 禁用 SS0 */
    ADC0_EMUX &= ~0x0000000F;     /* 软件触发 */
    ADC0_SSMUX0 = channel;        /* 设置通道（AIN0-3） */
    ADC0_SSCTL0 = 0x00000006;     /* 单次采样，触发中断 */
    ADC0_ACTSS |= 0x00000001;     /* 启用 SS0 */
}

/* ========================== 触发采样并读取数据 ========================== */
uint32_t adc_read(uint32_t channel) {
    if (channel > ADC_CHANNEL_3) return 0;

    /* [QEMU 有效] 触发采样 */
    ADC0_PSSI |= 0x00000001;      /* 启动 SS0 采样 */

    /* [QEMU 有效] 等待采样完成 */
    while (ADC0_SSFIFO0 & 0x10000); /* 检查 FIFO 空标志 */

    /* [QEMU 有效] 读取数据 */
    return ADC0_SSFIFO0 & 0x3FF;  /* 10 位数据 */
}

/* ========================== 注册中断处理函数（QEMU 无效） ========================== */
void adc_register_handler(uint32_t channel, void (*handler)(void)) {
    if (channel > ADC_CHANNEL_3) return;

    adc0_handler = handler;

    /* [QEMU 无效] 使能 SS0 中断 */
    ADC0_IM |= 0x00000001;

    /* [QEMU 无效] 启用 NVIC 中 ADC0 SS0 中断（IRQ#14） */
    NVIC_EN0 |= (1 << 14);
}

/* ========================== ADC0 中断处理函数（QEMU 无效） ========================== */
void ADC0_Handler(void) {
    if (adc0_handler) {
        adc0_handler();
    }

    /* [QEMU 无效] 清除中断标志 */
    ADC0_ICR = 0x00000001;
}