#ifndef UART_H
#define UART_H

#include <stdint.h>
#define UART_0 0

typedef struct
{
    uint32_t baudrate; 
    uint8_t port;       
} UART_Config;


/* 初始化 UART0，设置波特率 */
void uart_init(uint32_t baud);

/* 发送单个字符 */
void uart_send_char(char c);

/* 发送字符串 */
void uart_send_string(const char *str);

/* ========================== 注册中断处理函数（QEMU 无效） ========================== */
void uart_register_handler(void (*handler)(void));

/* ========================== UART0 中断处理函数（QEMU 无效） ========================== */
void UART0_Handler(void) ;

void uart_putc(char c);

#endif /* UART_H */