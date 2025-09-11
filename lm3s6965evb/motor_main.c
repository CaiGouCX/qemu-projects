#include <stdint.h>
#include <rtthread.h>

#include "lm3s6965evb.h"
#include "motor_control.h"


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
            //float speed = PID_Compute(setpoint, (float)current_position, pid_params, &state);
            //Actuator_Output(speed);
        }
        Send_Status();
        rt_thread_mdelay(10);
    }
}

/* 使用 INIT_APP_EXPORT 注册任务，RT-Thread 启动后会自动调用 */
INIT_APP_EXPORT(motor_task);

