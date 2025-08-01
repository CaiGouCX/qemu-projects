#include "pwm.h"

/* ========================== 寄存器定义 ========================== */
#define SYSCTL_BASE         0x400FE000
#define SYSCTL_RCGCPWM      (*((volatile uint32_t *)(SYSCTL_BASE + 0x640)))

#define PWM0_BASE           0x40028000
#define PWM0_CTL            (*((volatile uint32_t *)(PWM0_BASE + 0x008)))
#define PWM0_0_GENA         (*((volatile uint32_t *)(PWM0_BASE + 0x060)))
#define PWM0_0_LOAD         (*((volatile uint32_t *)(PWM0_BASE + 0x050)))
#define PWM0_0_CMPA         (*((volatile uint32_t *)(PWM0_BASE + 0x058)))
#define PWM0_ENABLE         (*((volatile uint32_t *)(PWM0_BASE + 0x040)))
#define PWM0_INTEN          (*((volatile uint32_t *)(PWM0_BASE + 0x004)))
#define PWM0_RIS            (*((volatile uint32_t *)(PWM0_BASE + 0x00C)))
#define PWM0_ISC            (*((volatile uint32_t *)(PWM0_BASE + 0x010)))

#define NVIC_EN0            (*((volatile uint32_t *)(0xE000E100)))

/* ========================== 中断处理函数指针 ========================== */
static void (*pwm0_handler)(void) = 0;

/* ========================== PWM 初始化 ========================== */
void pwm_init(uint32_t freq) {
    /* [QEMU 无效] 使能 PWM0 模块时钟 */
    SYSCTL_RCGCPWM |= 0x00000001;

    /* [QEMU 无效] 等待时钟稳定 */
    for (volatile int i = 0; i < 1000; i++);

    /* [QEMU 有效] 配置 PWM0 生成器 0 */
    PWM0_CTL = 0x00000000;        /* 禁用 PWM0 */
    PWM0_0_LOAD = (16000000 / freq) - 1; /* 设置周期，16MHz 系统时钟 */
    PWM0_0_CMPA = 0;              /* 初始占空比 0% */
    PWM0_0_GENA = 0x0000008C;     /* 向下计数，高电平当计数 < CMPA */
    PWM0_CTL = 0x00000001;        /* 启用 PWM0 */
    PWM0_ENABLE |= 0x00000001;    /* 启用 PWM0 输出 */
}

/* ========================== 设置占空比 ========================== */
void pwm_set_duty(uint32_t duty) {
    if (duty > 100) duty = 100;   /* 限制占空比 0-100% */

    /* [QEMU 有效] 设置比较值 */
    uint32_t load = PWM0_0_LOAD;
    PWM0_0_CMPA = (load * duty) / 100; /* 占空比计算 */
}

/* ========================== 注册中断处理函数（QEMU 无效） ========================== */
void pwm_register_handler(void (*handler)(void)) {
    pwm0_handler = handler;

    /* [QEMU 无效] 使能 PWM0 生成器 0 中断 */
    PWM0_INTEN |= 0x00000001;

    /* [QEMU 无效] 启用 NVIC 中 PWM0 中断（IRQ#10） */
    NVIC_EN0 |= (1 << 10);
}

/* ========================== PWM0 中断处理函数（QEMU 无效） ========================== */
void PWM0_Handler(void) {
    if (pwm0_handler) {
        pwm0_handler();
    }

    /* [QEMU 无效] 清除中断标志 */
    PWM0_ISC = 0x00000001;
}