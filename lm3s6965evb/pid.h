#ifndef __PID_H__
#define __PID_H__
#include "motor_control.h"
void Actuator_Output(int16_t speed_q8_8);
int16_t PID_Compute(int16_t setpoint, int16_t current,
                        const int16_t *params,
                        volatile MotorState state);

float PID_Compute_Current(float setpoint, float measured, float *pid_params, volatile MotorState state);
void Actuator_Output_Current(float current);
#endif