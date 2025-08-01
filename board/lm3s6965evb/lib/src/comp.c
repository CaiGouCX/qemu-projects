#include "comp.h"

/* ========================== 寄存器定义 ========================== */
#define SYSCTL_BASE         0x400FE000
#define SYSCTL_RCGCACMP     (*((volatile uint32_t *)(SYSCTL_BASE + 0x63C)))

#define COMP_BASE           0x4000C000
#define COMP_ACCTL0         (*((volatile uint32_t *)(COMP_BASE + 0x024)))
#define COMP_ACMIS          (*((volatile uint32_t *)(COMP_BASE + 0x004)))
#define COMP_ACINTEN        (*((volatile uint32_t *)(COMP_BASE + 0x008)))
#define COMP_ACRIS          (*((volatile uint32_t *)(COMP_BASE + 0x00C)))

#define NVIC_EN0            (*((volatile uint32_t *)(0xE000E100)))

/* ========================== 中断处理函数指针 ========================== */
static void (*comp0_handler)(void) = 0;

/* ========================== Comparator 初始化 ========================== */
void comp_init(float ref_voltage) {
    /* [QEMU 无效] 使能 Comparator 模块时钟 */
    SYSCTL_RCGCACMP |= 0x00000001;

    /* [QEMU 无效] 等待时钟稳定 */
    for (volatile int i = 0; i < 1000; i++);

    /* [QEMU 有效] 配置 Comparator0 */
    uint32_t ref = (uint32_t)(ref_voltage / 3.3 * 7); /* 0-7 对应 0-3.3V */
    if (ref > 7) ref = 7;
    COMP_ACCTL0 = (1 << 9) | (ref << 6); /* 启用比较器，使用内部参考电压 */
}

/* ========================== 获取比较器状态 ========================== */
uint32_t comp_get_status(void) {
    /* [QEMU 有效] 返回比较器输出状态 */
    return (COMP_ACMIS & 0x00000001) ? 1 : 0;
}

/* ========================== 注册中断处理函数（QEMU 无效） ========================== */
void comp_register_handler(void (*handler)(void)) {
    comp0_handler = handler;

    /* [QEMU 无效] 使能 Comparator0 中断 */
    COMP_ACINTEN |= 0x00000001;

    /* [QEMU 无效] 启用 NVIC 中 Comparator 中断（IRQ#4） */
    NVIC_EN0 |= (1 << 4);
}

/* ========================== Comparator 中断处理函数（QEMU 无效） ========================== */
void COMP0_Handler(void) {
    if (comp0_handler) {
        comp0_handler();
    }

    /* [QEMU 无效] 清除中断标志 */
    COMP_ACRIS = 0x00000001;
}