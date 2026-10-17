/*
 * motor.c
 *
 *  Created on: Oct 15, 2026
 *      Author: aymen
 */

#include "motor.h"
#include <math.h>    /* for fabsf() */
#include "config.h"

void Motor_Set(Motor *m, float duty)
{
	if (duty > MAX_DUTY) duty = MAX_DUTY;
	if (duty < -MAX_DUTY) duty = -MAX_DUTY;
    m->duty = duty;
    if (m->invert == 1) duty = -duty;
    if (duty > 0.0f) {
        HAL_GPIO_WritePin(m->in1_port, m->in1_pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(m->in2_port, m->in2_pin, GPIO_PIN_RESET);
    } else if (duty < 0.0f) {
        HAL_GPIO_WritePin(m->in1_port, m->in1_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(m->in2_port, m->in2_pin, GPIO_PIN_SET);
    }
    uint32_t ccr = (uint32_t)(fabsf(duty) * 9000);
    __HAL_TIM_SET_COMPARE(m->htim, m->channel, ccr);
}

void Motor_Coast(Motor *m)
{
    m->duty = 0.0f;
    __HAL_TIM_SET_COMPARE(m->htim, m->channel, 0);
    HAL_GPIO_WritePin(m->in1_port, m->in1_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(m->in2_port, m->in2_pin, GPIO_PIN_RESET);
}
void Motor_Init(Motor *m)
{
    __HAL_TIM_SET_COMPARE(m->htim, m->channel, 0);
    HAL_TIM_PWM_Start(m->htim, m->channel);
    Motor_Coast(m);
}
void Motor_Standby(int enable_driver){
	if (enable_driver == 1){
		HAL_GPIO_WritePin(STBY_GPIO_Port, STBY_Pin, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(STBY_GPIO_Port, STBY_Pin, GPIO_PIN_RESET);
	}
}
