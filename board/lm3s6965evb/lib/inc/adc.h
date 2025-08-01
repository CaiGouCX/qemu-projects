#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/* ADC 通道定义 */
#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1
#define ADC_CHANNEL_2 2
#define ADC_CHANNEL_3 3

/* 初始化 ADC 通道（SS0） */
void adc_init(uint32_t channel);

/* 触发采样并读取数据 */
uint32_t adc_read(uint32_t channel);

/* 注册中断处理回调（QEMU 无效） */
void adc_register_handler(uint32_t channel, void (*handler)(void));

/* ADC0 中断处理函数（QEMU 无效） */
void ADC0_Handler(void);

#endif /* ADC_H */