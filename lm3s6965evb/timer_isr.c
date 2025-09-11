#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>
#include "motor_control.h"
#include "pid.h"

// 模拟定时器寄存器结构体
typedef struct {
    volatile uint32_t CTL;  // 控制寄存器
    volatile uint32_t CFG;  // 配置寄存器
    volatile uint32_t TAMR; // Timer A 模式
    volatile uint32_t TAILR;// Timer A 加载值
    volatile uint32_t IMR;  // 中断使能
    volatile uint32_t RIS;  // 原始中断状态
    volatile uint32_t ICR;  // 中断清除
} TIMER0_Type;

// 模拟宏定义
#define TIMER0_BASE             0x40030000
#define TIMER0                  ((TIMER0_Type *) TIMER0_BASE)
#define TIMER_RIS_TATORIS       0x00000001  // Timer A 超时中断标志
#define TIMER_ICR_TATOCINT      0x00000001  // 清除 Timer A 超时中断
#define TIMER_CTL_TAEN          0x00000001  // 启用 Timer A
#define TIMER_TAMR_TAMR_PERIOD  0x00000002  // 周期模式
#define TIMER_IMR_TATOIM        0x00000001  // 启用 Timer A 超时中断

// 模拟 SYSCTL 和 NVIC
typedef struct {
    volatile uint32_t RCGC1;  // 时钟门控控制
} SYSCTL_Type;
#define SYSCTL_BASE             0x400FE000
#define SYSCTL                  ((SYSCTL_Type *) SYSCTL_BASE)
#define SYSCTL_RCGC1_TIMER0     0x00010000  // Timer0 时钟门控

// 模拟 NVIC 寄存器
typedef struct {
    volatile uint32_t ISER[32];  // 中断使能
    volatile uint32_t IP[32];    // 中断优先级
} NVIC_Type;
#define NVIC                    ((NVIC_Type *) 0xE000E100)
#define TIMER0A_IRQn            19  // Timer0 A 中断号



void TIMER0A_Handler(void)  // 标准 CMSIS 风格 ISR 名称
{
    // 1. 确认并清除中断标志
    if (TIMER0->RIS & TIMER_RIS_TATORIS)   // Timer A 超时中断
    {
        TIMER0->ICR = TIMER_ICR_TATOCINT;  // 清除中断标志

        // 2. 在中断里跑速度环 PID（快速运算）
        float speed = PID_Compute(setpoint, current_position, pid_params, state);

        // 3. 更新执行器（如 PWM 占空比）
        Actuator_Output(speed);
    }
}




rt_isr_handler_t rt_hw_interrupt_install(int vector,
                                         rt_isr_handler_t handler,
                                         void *param,
                                         const char *name)
{
    /* 临时 stub：直接返回传入的 handler，实际功能未实现 */
    return handler;
}
void timer0_init(void)
{
    // 模拟硬件初始化（空操作）
    rt_kprintf("Timer0 initialized in QEMU\n");
    // 注册中断处理程序
    rt_hw_interrupt_install(TIMER0A_IRQn, TIMER0A_Handler, RT_NULL, "TIMER0A_Handler");
    // 模拟中断使能
}