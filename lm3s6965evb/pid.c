#include <stdint.h>
#include "pid.h"
#include "motor_control.h"

/**************************************************************
 * pid.c
 * 三种 PID 实现版本：
 * 1. 定点 Q15（无 FPU，适合 Cortex-M3 高频 ISR）
 * 2. FPU 浮点（有硬件 FPU，如 Cortex-M4/M7）
 * 3. 软件浮点（无硬件 FPU，仿真或低频环）
 **************************************************************/

/* ------------------ 通用状态定义 ------------------ */
#define STATE_RUN   0
#define STATE_HOLD  1

/* ------------------ 定点 Q15 版本 ------------------ */
#define PID_SCALE   1024
#define PID_SHIFT   10


/**
 * @brief  定点 Q15 PID 计算 (Cortex-M3 无 FPU)
 * @param  setpoint: 目标值 (Q15)
 * @param  current: 当前值 (Q15)
 * @param  params: PID 参数 [Kp, Ki, Kd] Q15
 * @param  state: 电机状态指针
 * @param  pid_state: PID 状态结构体指针
 * @retval int16_t: 输出值 (Q15)
 * 
 * 优化点：
 * 1. 整数运算代替浮点，CPU 周期少
 * 2. 积分限幅防止溢出
 * 3. 位移代替除法，低开销
 */
int16_t PID_Compute(int16_t setpoint, int16_t current,
                        const int16_t *params,
                        volatile MotorState state)

{
    // 静态变量替代 PID_State_Q15
    static int32_t integral = 0;
    static int32_t prev_error = 0;

    int32_t error = (int32_t)setpoint - (int32_t)current;

    integral += error;
    if(integral > (32767<<PID_SHIFT)) integral = (32767<<PID_SHIFT);
    if(integral < (-32768<<PID_SHIFT)) integral = (-32768<<PID_SHIFT);

    int32_t derivative = error - prev_error;
    prev_error = error;

    int32_t output = params[0]*error + params[1]*integral + params[2]*derivative;
    output >>= PID_SHIFT;

    if(state == HOLD) output = 0;

    if(output > 32767) output = 32767;
    if(output < -32768) output = -32768;

    return (int16_t)output;
}


/* ------------------ FPU 浮点版本 ------------------ */
/**
 * @brief  硬件 FPU PID (Cortex-M4/M7)
 * @param  setpoint: 目标值
 * @param  current: 当前值
 * @param  params: PID 参数 [Kp, Ki, Kd]
 * @param  state: 电机状态指针
 * @param  integral: 积分变量指针
 * @param  prev_error: 上一次误差指针
 * @retval float: 输出值
 *
 * 优化点：
 * 1. 使用硬件 FPU，乘加速度快
 * 2. 保留积分/微分变量可避免函数内 static，方便多实例
 */
float PID_Compute_FPU(float setpoint, float current, const float *params, volatile MotorState state)
{
    static float integral = 0.0f;
    static float prev_error = 0.0f;

    float error = setpoint - current;
    integral += error;
    float derivative = error - prev_error;
    prev_error = error;

    float output = params[0]*error + params[1]*integral + params[2]*derivative;

    if(state == HOLD) output = 0.0f;

    return output;
}

/* ------------------ 软件浮点版本 ------------------ */
/**
 * @brief  软件浮点 PID (无 FPU)
 * @param  setpoint: 目标值
 * @param  current: 当前值
 * @param  params: PID 参数 [Kp, Ki, Kd]
 * @param  state: 电机状态指针
 * @retval float: 输出值
 *
 * 优化点：
 * 1. 简单直观，可仿真或低频使用
 * 2. 每次调用使用 static 变量，保存积分与微分
 */
float fabsf(float speed){
    if (speed < 0){
        return -speed ;
    }else{
        return speed ;
    }
}

void Actuator_Output_FPU(float speed)
{
    // 优化点：1) 限幅使用 else if 避免两次比较
    if(speed > 100.0f) speed = 100.0f;
    else if(speed < -100.0f) speed = -100.0f;

    // 优化点：2) 使用硬件 FPU 的 fabsf 计算绝对值
    PWM0_SetDuty((uint16_t)(fabsf(speed) * 10.0f)); // 假设 PWM 占空比 0~1000
}

/* ================== 2. 软件浮点版 ================== */
void Actuator_Output_SW(float speed)
{
    // 优化点：1) 限幅使用 else if
    if(speed > 100.0f) speed = 100.0f;
    else if(speed < -100.0f) speed = -100.0f;

    // 优化点：2) 无 FPU，手动计算绝对值
    float abs_speed = (speed >= 0.0f) ? speed : -speed;

    PWM0_SetDuty((uint16_t)(abs_speed * 10.0f)); // PWM 占空比 0~1000
}

/* ================== 3. 定点 Q8.8 版本 ================== */
void Actuator_Output(int16_t speed_q8_8)
{
    // 优化点：1) 限幅使用 else if
    if(speed_q8_8 > 100*256) speed_q8_8 = 100*256;
    else if(speed_q8_8 < -100*256) speed_q8_8 = -100*256;

    // 优化点：2) 整数计算绝对值，避免浮点
    int16_t abs_speed = (speed_q8_8 >= 0) ? speed_q8_8 : -speed_q8_8;

    // 优化点：3) Q8.8 转 PWM 占空比，位移计算快速
    PWM0_SetDuty((uint16_t)((abs_speed * 10) >> 8)); // Q8.8 -> 0~1000
}


/*        电流环            */

float PID_Compute_Current(float setpoint, float measured, float *pid_params, volatile MotorState state)
{
    static int32_t integral = 0;
    static int32_t last_error = 0;
    float error = setpoint - measured;
    float Kp = pid_params[0], Ki = pid_params[1], Kd = pid_params[2];
    integral += error;
    float derivative = error - last_error;
    float output = Kp * error + Ki * integral + Kd * derivative;
    last_error = error;
    return output; // 电流环输出
}

void Actuator_Output_Current(float current)
{
    rt_kprintf("Current output: %f\n", current); // 模拟电流执行器
}