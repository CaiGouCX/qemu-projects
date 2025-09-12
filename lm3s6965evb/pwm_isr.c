#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>

#include "pid.h"
#include "motor_control.h"
#include "foc.h"

// 模拟 PWM 寄存器结构体
typedef struct {
    volatile uint32_t CTL;   // 控制寄存器
    volatile uint32_t GEN0;  // 生成器 0
    volatile uint32_t LOAD0; // 加载值 0
    volatile uint32_t IMR;   // 中断使能
    volatile uint32_t RIS;   // 原始中断状态
    volatile uint32_t ICR;   // 中断清除
} PWM0_Type;

// 模拟宏定义
#define PWM0_BASE               0x40028000  // LM3S6965 PWM0 基地址
#define PWM0                    ((PWM0_Type *) PWM0_BASE)
#define PWM_RIS_GEN0            0x00000001  // 生成器 0 中断标志
#define PWM_ICR_GEN0            0x00000001  // 清除生成器 0 中断
#define PWM_CTL_ENABLE          0x00000001  // 启用 PWM
#define PWM_GEN0_MODE_PERIOD    0x00000002  // 周期模式
#define PWM_IMR_GEN0            0x00000001  // 启用生成器 0 中断

// 模拟 SYSCTL 和 NVIC
typedef struct {
    volatile uint32_t RCGC1;  // 时钟门控控制
} SYSCTL_Type;
#define SYSCTL_BASE             0x400FE000
#define SYSCTL                  ((SYSCTL_Type *) SYSCTL_BASE)
#define SYSCTL_RCGC1_PWM0       0x00100000  // PWM0 时钟门控

typedef struct {
    volatile uint32_t ISER[32];  // 中断使能
    volatile uint32_t IP[32];    // 中断优先级
} NVIC_Type;
#define NVIC                    ((NVIC_Type *) 0xE000E100)
#define PWM0_IRQn               45  // PWM0 中断号

// 电流环变量
volatile float setpoint_current;    // 电流设定点
volatile float current_feedback;    // 当前电流反馈
volatile float pid_params_current[3]; // 电流环 PID 参数 (Kp, Ki, Kd)
volatile MotorState state_current;


void PWM0_Handler(void)
{   static FOC_State foc_state;//本来是foc.h下定义extern FOC_State foc_state;此次单纯为了解决空间不足过编译，实际可优化全局变量
    rt_base_t level = rt_hw_interrupt_disable();
    if (PWM0->RIS & PWM_RIS_GEN0)
    {
        PWM0->ICR = PWM_ICR_GEN0;
        // 模拟三相电流和转子角度（QEMU）
        foc_state.ia = current_feedback;
        foc_state.ib = -0.5f * current_feedback;
        foc_state.ic = -0.5f * current_feedback;
        foc_state.theta += 0.1f; // 模拟角度增量
        // FOC 控制
        FOC_Control(foc_state.ia, foc_state.ib, foc_state.ic, foc_state.theta, 0.0f, setpoint_current);
    }
    rt_hw_interrupt_enable(level);
}

void pwm0_init(void)
{
    rt_kprintf("PWM0 initialized in QEMU\n");
    init_trig_lut(); // 初始化查找表
    rt_hw_interrupt_install(PWM0_IRQn, PWM0_Handler, RT_NULL, "PWM0_Handler");
}