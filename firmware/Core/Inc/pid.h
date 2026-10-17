/*
 * pid.h
 *
 *  Created on: Oct 15, 2026
 *      Author: aymen
 */

#ifndef INC_PID_H
#define INC_PID_H

typedef struct {
    float kp;          /* P gain                                   */
    float ki;          /* I gain                                   */
    float kd;        /* D gain                       (blank 1)   */
    float kff;         /* starting guess: duty per RPM             */
    float integ;       /* the I term's running total               */
    float prev_meas;   /* last tick's measured speed (for D)       */
} PID;

void  PID_Init(PID *p, float kp, float ki, float kd, float kff);   /* (blank 2) */
void  PID_Reset(PID *p, float meas);
float PID_Update(PID *p, float setpoint, float meas, float dt,      /* (blank 3) */
                 float out_min, float out_max);

#endif
