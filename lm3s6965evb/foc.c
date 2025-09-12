#include <rtthread.h>
#include "foc.h"
// 查找表（简化版，360 度分成 360 点）
#define LUT_SIZE 360
static float sin_lut[LUT_SIZE];
static float cos_lut[LUT_SIZE];

static FOC_State foc_state;
/********** space func *********** */
float sinf(float a){}
float cosf(float b){}
float sqrtf(float c){}
float atan2f(float d,float e){}
/********************************************* */
void init_trig_lut(void)
{
    for (int i = 0; i < LUT_SIZE; i++) {
        float angle = (i * 2.0f * M_PI) / LUT_SIZE;
        sin_lut[i] = sinf(angle);
        cos_lut[i] = cosf(angle);
    }
}

float fast_sin(float theta)
{
    int index = (int)(theta * LUT_SIZE / (2.0f * M_PI)) % LUT_SIZE;
    return sin_lut[index];
}

float fast_cos(float theta)
{
    int index = (int)(theta * LUT_SIZE / (2.0f * M_PI)) % LUT_SIZE;
    return cos_lut[index];
}

void Clark_Transform(float ia, float ib, float ic, float *alpha, float *beta)
{
    *alpha = ia - 0.5f * ib - 0.5f * ic;
    *beta = (0.8660254f) * (ib - ic); // sqrt(3)/2
}

void Park_Transform(float alpha, float beta, float theta, float *d, float *q)
{
    float cos_theta = fast_cos(theta);
    float sin_theta = fast_sin(theta);
    *d = alpha * cos_theta + beta * sin_theta;
    *q = -alpha * sin_theta + beta * cos_theta;
}

void Inverse_Park_Transform(float d, float q, float theta, float *alpha, float *beta)
{
    float cos_theta = fast_cos(theta);
    float sin_theta = fast_sin(theta);
    *alpha = d * cos_theta - q * sin_theta;
    *beta = d * sin_theta + q * cos_theta;
}

void SVPWM(float alpha, float beta, float *va, float *vb, float *vc)
{
    float vref = sqrtf(alpha * alpha + beta * beta);
    float angle = atan2f(beta, alpha);
    *va = 0.5f * (1.0f + vref * fast_cos(angle));
    *vb = 0.5f * (1.0f + vref * fast_cos(angle - 2.0f * M_PI / 3.0f));
    *vc = 0.5f * (1.0f + vref * fast_cos(angle + 2.0f * M_PI / 3.0f));
    rt_kprintf("SVPWM: va=%f, vb=%f, vc=%f\n", *va, *vb, *vc);
}

void FOC_Control(float ia, float ib, float ic, float theta, float id_ref, float iq_ref)
{
    Clark_Transform(ia, ib, ic, &foc_state.alpha, &foc_state.beta);
    Park_Transform(foc_state.alpha, foc_state.beta, theta, &foc_state.id, &foc_state.iq);
    foc_state.vd = PID_Compute(id_ref, foc_state.id, foc_state.pid_params_id, &foc_state.state_id);
    foc_state.vq = PID_Compute(iq_ref, foc_state.iq, foc_state.pid_params_iq, &foc_state.state_iq);
    Inverse_Park_Transform(foc_state.vd, foc_state.vq, theta, &foc_state.alpha, &foc_state.beta);
    SVPWM(foc_state.alpha, foc_state.beta, &foc_state.va, &foc_state.vb, &foc_state.vc);
}