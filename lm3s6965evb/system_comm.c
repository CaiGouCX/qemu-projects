/* system_comm.c: 系统初始化与 SPI 命令解析、中断调度 */

#include <stdint.h>
#include "lm3s6965evb.h"
#include "motor_control.h"  // 控制状态与错误类型

extern void SSI0_Handler(void);

// ✅ 有效：系统初始化（被 main 调用）
void System_Init(void) {
    SysCtrl_ClockSet(50000000);   // ✅ 设置时钟为 50 MHz（QEMU 有效）

    PWM0_Init(1000);              // ⚠️ QEMU 中 PWM 模块行为不可见，但保留
    QEI0_Init();                  // ⚠️ QEMU 无 QEI 输入设备，但保留位置结构
    Timer0_Init(1000);           // ⚠️ QEMU 中定时器中断不一定生效
    SSI0_Init(1000000);           // ✅ 可用作 SPI 模拟或串口替代（如用 UART 重定向）

    nvic_enable_irq(23);          // ⚠️ QEMU 中断无效，但函数存在必要
    nvic_enable_irq(19);

    state = HOLD;
    error = NO_ERROR;
}

// ✅ 有效：定时器中断服务函数（Timer0A IRQ#19）
void Timer0A_Handler(void) {
    current_position = QEI0_GetPosition();     // ⚠️ QEMU 无 QEI 编码器，模拟返回值
    Timer0_ClearInterrupt();                   // ⚠️ 清中断标志（无真实触发）

    if (current_position == 0xFFFFFFFF) {
        error = QEI_FAULT;                     // 模拟故障情况
    }
}

// ✅ 有效：SPI 接收命令解析，简化协议实现
float Parse_Command(uint8_t *buffer, uint32_t len) {
    float setpoint = 0.0;

    if (len < 5) {
        error = BUFFER_OVERFLOW;
        return setpoint;
    }

    // 示例协议："CMD:5:<CRC>"
    if (buffer[0] == 'C' && buffer[1] == 'M' && buffer[2] == 'D') {
        setpoint = (float)(buffer[4] - '0'); // 实际应解析字符串为 float

        // 简单的 checksum 校验
        uint8_t crc = 0;
        for (uint32_t i = 0; i < len - 1; i++) {
            crc += buffer[i];
        }

        if (crc == buffer[len - 1]) {
            state = MOVE;
            error = NO_ERROR;
            SSI0_Send('O'); SSI0_Send('K');   // ✅ 返回 OK
        } else {
            error = CRC_ERROR;
            SSI0_Send('E'); SSI0_Send('R');   // ✅ 返回 ER
        }
    }

    return setpoint;
}

// ✅ 有效：定时状态发送，用于主机监测状态
void Send_Status(void) {
    uint8_t status[10];

    status[0] = 'S'; status[1] = 'T'; status[2] = 'S'; status[3] = ':';
    status[4] = (uint8_t)(current_position & 0xFF);
    status[5] = state == HOLD ? 'H' : 'M';
    status[6] = ':'; status[7] = (uint8_t)error;

    uint8_t crc = 0;
    for (uint32_t i = 0; i < 8; i++) crc += status[i];
    status[8] = crc;

    for (uint32_t i = 0; i < 9; i++) {
        SSI0_Send(status[i]);  // ✅ 通过 SPI (或替代串口) 返回
    }
}
