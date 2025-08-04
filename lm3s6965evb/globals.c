// globals.c
#include "motor_control.h"  // 确保包含声明头文件

// 实际定义所有全局变量（分配内存）
uint8_t comm_buffer[64] = {0};            // 命令缓冲区
uint32_t position_data[16] = {0};         // QEI 数据
float pid_params[3] = {0.0f, 0.0f, 0.0f}; // PID 参数 (Kp, Ki, Kd)
volatile uint32_t comm_index = 0;          // 命令索引
volatile uint8_t comm_ready = 0;           // 命令就绪
volatile uint32_t current_position = 0;    // 当前位置
volatile float setpoint = 0.0f;            // 目标位置
volatile MotorState state = HOLD;          // 电机状态 (初始为HOLD)
volatile ErrorCode error = NO_ERROR;       // 错误状态 (初始为NO_ERROR)