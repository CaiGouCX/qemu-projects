/* motor_control.h: 电机控制共享定义 */
#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <stdint.h>

/* 执行器状态 */
typedef enum { MOVE, HOLD } MotorState;

/* 错误代码 */
typedef enum { NO_ERROR, CRC_ERROR, BUFFER_OVERFLOW, QEI_FAULT } ErrorCode;

/* 全局变量，分配在 .bss */
extern uint8_t comm_buffer[64];      // 命令缓冲区
extern uint32_t position_data[16];   // QEI 数据
extern float pid_params[3];          // PID 参数 (Kp, Ki, Kd)
extern volatile uint32_t comm_index; // 命令索引
extern volatile uint8_t comm_ready;  // 命令就绪
extern volatile uint32_t current_position; // 当前位置
extern volatile float setpoint;       // 目标位置
extern volatile MotorState state;     // 电机状态
extern volatile ErrorCode error;      // 错误状态

/* 函数原型 */
void System_Init(void);
float Parse_Command(uint8_t *buffer, uint32_t len);
void Send_Status(void);

#endif /* MOTOR_CONTROL_H */