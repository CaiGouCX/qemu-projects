#ifndef __FOC_H__
#define __FOC_H__

#include "motor_control.h"

#define M_PI 3.14

typedef struct {
    float theta;      // 转子角度
    float ia, ib, ic; // 三相电流
    float id, iq;     // d-q 电流
    float vd, vq;     // d-q 电压
    float alpha, beta;// α-β 电压
    float va, vb, vc; // 三相 PWM 占空比
    float pid_params_id[3]; // Id PID 参数
    float pid_params_iq[3]; // Iq PID 参数
    MotorState state_id;    // Id PID 状态
    MotorState state_iq;    // Iq PID 状态
} FOC_State;



void init_trig_lut(void);

float fast_sin(float theta);

float fast_cos(float theta);

void Clark_Transform(float ia, float ib, float ic, float *alpha, float *beta);

void Park_Transform(float alpha, float beta, float theta, float *d, float *q);

void Inverse_Park_Transform(float d, float q, float theta, float *alpha, float *beta);

void SVPWM(float alpha, float beta, float *va, float *vb, float *vc);

void FOC_Control(float ia, float ib, float ic, float theta, float id_ref, float iq_ref);
#endif