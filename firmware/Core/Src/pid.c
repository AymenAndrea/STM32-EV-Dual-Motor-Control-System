/*
 * pid.c
 *
 *  Created on: Oct 15, 2026
 *      Author: aymen
 */
#include "pid.h"

void PID_Init(PID *p, float kp, float ki, float kd, float kff)
{
    p->kp  = kp;
    p->ki  = ki;
    p->kd  = kd;
    p->kff = kff;
    PID_Reset(p, 0.0f);
}

void PID_Reset(PID *p, float meas)
{
    p->integ     = 0.0f;
    p->prev_meas = meas;
}

float PID_Update(PID *p, float setpoint, float meas, float dt,
                 float out_min, float out_max)
{
    float err   = setpoint - meas;
    float pterm = p->kp * err;
    float ff    = p->kff * setpoint;
    float i_new = p->integ + p->ki * err * dt;
    float dterm = -p->kd * (meas - p->prev_meas) / dt;
    p->prev_meas = meas;
    float u = ff + pterm + i_new + dterm;

    if (u > out_max) {
        u = out_max;
    } else if (u < out_min) {
        u = out_min;
    } else {
        p->integ = i_new;
    }
    if (p->integ >  1.0f) p->integ =  1.0f;
    if (p->integ < -1.0f) p->integ = -1.0f;
    return u;
}
