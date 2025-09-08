#ifndef SYSCTRL_H
#define SYSCTRL_H

#include <stdint.h>
#include <stdbool.h>
/* 外设定义 */
#define SYSCTRL_PERIPH_UART0  0
#define SYSCTRL_PERIPH_SSI0   1
#define SYSCTRL_PERIPH_I2C0   2
#define SYSCTRL_PERIPH_PWM0   3
#define SYSCTRL_PERIPH_ADC0   4
#define SYSCTRL_PERIPH_GPIOA  5

/* 时钟源选择 */
typedef enum
{
    SYSCTRL_OSC_MAIN = 0,   /* 外部主晶振 */
    SYSCTRL_OSC_INT,        /* 内部振荡器 */
    SYSCTRL_OSC_INT_30KHZ   /* 内部低速30kHz */
} SysCtrl_OscSource;

/* 系统控制配置结构体 */
typedef struct
{
    SysCtrl_OscSource osc_source; /* 振荡器来源 */
    bool use_pll;                 /* 是否启用 PLL */
    uint8_t crystal_value;        /* 晶振频率编码，参考手册 RCC 寄存器 */
    uint8_t sysdiv;               /* 系统分频值 (0-15)，0表示不分频 */
} SysCtrl_Config;

/* 初始化系统时钟（默认 16MHz） */
void sysctrl_init(SysCtrl_Config *cfg);

/* 使能外设时钟 */
void sysctrl_enable_periph(uint32_t periph);

/* 复位外设 */
void sysctrl_reset_periph(uint32_t periph);

/* 注册中断处理回调（QEMU 无效） */
void sysctrl_register_handler(void (*handler)(void));

/* SysCtrl 中断处理函数（QEMU 无效） */
void SYSCTL_Handler(void);

uint32_t SysCtrl_GetSysClockFreq(void);
uint32_t SysTick_Config(uint32_t ticks);
#endif /* SYSCTRL_H */