#include <stdint.h>

/* LM3S6965 寄存器定义 */
#define SYSCTL_BASE       0x400FE000
#define SYSCTL_RCC        (*((volatile uint32_t *)(SYSCTL_BASE + 0x060)))
#define SYSCTL_RCC2       (*((volatile uint32_t *)(SYSCTL_BASE + 0x070)))
#define SYSCTL_RCGC1      (*((volatile uint32_t *)(SYSCTL_BASE + 0x104)))
#define SYSCTL_RCGC2      (*((volatile uint32_t *)(SYSCTL_BASE + 0x108)))

#define GPIOA_BASE        0x40004000
#define GPIOA_DIR         (*((volatile uint32_t *)(GPIOA_BASE + 0x400)))
#define GPIOA_AFSEL       (*((volatile uint32_t *)(GPIOA_BASE + 0x420)))
#define GPIOA_DEN         (*((volatile uint32_t *)(GPIOA_BASE + 0x51C)))
#define GPIOA_PUR         (*((volatile uint32_t *)(GPIOA_BASE + 0x510)))  // 使用 PUR 替代 PCTL

#define UART0_BASE        0x4000C000
#define UART0_DR          (*((volatile uint32_t *)(UART0_BASE + 0x000)))
#define UART0_FR          (*((volatile uint32_t *)(UART0_BASE + 0x018)))
#define UART0_IBRD        (*((volatile uint32_t *)(UART0_BASE + 0x024)))
#define UART0_FBRD        (*((volatile uint32_t *)(UART0_BASE + 0x028)))
#define UART0_LCRH        (*((volatile uint32_t *)(UART0_BASE + 0x02C)))
#define UART0_CTL         (*((volatile uint32_t *)(UART0_BASE + 0x030)))


/* 系统初始化函数 */
void SystemInit(void) 
{
    /* 0. 配置系统时钟 - 修复定时器报错 */
    SYSCTL_RCC |= (1 << 11);      // 使用主振荡器
    SYSCTL_RCC2 |= (1 << 31);     // 启用 RCC2
    SYSCTL_RCC2 &= ~0x1C000000;   // 清除 OSCSRC2
    SYSCTL_RCC2 |= 0x00000000;    // 选择主振荡器
    SYSCTL_RCC2 &= ~0x00007000;   // 清除分频器
    SYSCTL_RCC2 |= 0x00004000;    // 分频器设置为 /2
    
    /* 1. 使能 UART0 和 GPIOA 时钟 */
    SYSCTL_RCGC1 |= 0x00000001;  /* 使能 UART0 时钟 */
    SYSCTL_RCGC2 |= 0x00000001;  /* 使能 GPIOA 时钟 */
    
    /* 2. 配置 PA0 和 PA1 为 UART0 功能 - 修复 PL061 报错 */
    // 添加短暂延时确保外设时钟稳定
    volatile int i;
    for(i = 0; i < 1000; i++);
    
    GPIOA_DIR &= ~0x03;          /* PA0 和 PA1 设为输入 */
    GPIOA_AFSEL |= 0x03;         /* 启用 PA0 和 PA1 的复用功能 */
    // 避免使用 PCTL 寄存器 - 使用 PUR 替代
    GPIOA_PUR |= 0x03;           /* 启用上拉电阻 */
    GPIOA_DEN |= 0x03;           /* 使能数字功能 */
    
    /* 3. 配置 UART0 */
    UART0_CTL &= ~0x00000001;    /* 禁用 UART */
    UART0_IBRD = 8;              /* 波特率整数部分 (16MHz / (16 * 115200) = 8.68) */
    UART0_FBRD = 44;             /* 波特率小数部分 (0.68 * 64 + 0.5 = 44) */
    UART0_LCRH = 0x00000060;     /* 8位数据, 1位停止位, 无奇偶校验 */
    UART0_CTL |= 0x00000001;     /* 启用 UART */
}

/* 串口发送函数 - 针对 QEMU 优化 */
void uart_send_char(char c) {
    /* 等待发送缓冲区为空 */
    while (UART0_FR & 0x00000020); // 检查 TXFF 位
    UART0_DR = c;
}

/* 串口发送字符串 */
void uart_send_string(const char *str) {
    while(*str) {
        uart_send_char(*str++);
    }
}

/* 主函数 */
int main(void) {
    /* 初始化系统 */
    SystemInit();
    
    /* 发送欢迎消息 */
    uart_send_string("\n===== LM3S6965 QEMU Test =====\n");
    uart_send_string("Hello, QEMU Bare Metal!\n");
    uart_send_string("System initialized successfully\n");
    
    /* 添加退出机制 - 让程序进入无限循环 */
    while(1) {
        /* 什么都不做，保持程序运行 */
    }
    
    /* 或者更优雅的退出方式 - 通过系统调用退出 */
    // __asm__ volatile (
    //     "mov r0, #0\n"   // 退出码
    //     "mov r7, #1\n"   // SYS_exit 系统调用号
    //     "svc 0"          // 执行系统调用
    // );
}