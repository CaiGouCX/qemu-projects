#include <stdint.h>
#include <rtthread.h>

#include "lm3s6965evb.h"
#include "motor_control.h"

float PID_Compute(float setpoint, float current, float *params, volatile MotorState *state) {
    static float integral = 0.0f;
    static float prev_error = 0.0f;
    float error = setpoint - current;
    integral += error;
    float derivative = error - prev_error;
    float output = params[0] * error + params[1] * integral + params[2] * derivative;
    prev_error = error;
    if (*state == HOLD) output = 0.0f; // 保持状态输出 0
    return output;
}

void Actuator_Output(float speed) {
    if (speed > 100.0f) speed = 100.0f;
    if (speed < -100.0f) speed = -100.0f;
    PWM0_SetDuty(speed > 0 ? speed : -speed);
}


/* 电机控制任务 */
int motor_task(void *parameter)
{
    while (1)
    {
        if (comm_ready)
        {
            setpoint = Parse_Command(comm_buffer, comm_index);
            comm_ready = 0;
        }
        if (state == MOVE)
        {
            float speed = PID_Compute(setpoint, (float)current_position, pid_params, &state);
            Actuator_Output(speed);
        }
        Send_Status();
        rt_thread_mdelay(10);
    }
}

/* 使用 INIT_APP_EXPORT 注册任务，RT-Thread 启动后会自动调用 */
INIT_APP_EXPORT(motor_task);

