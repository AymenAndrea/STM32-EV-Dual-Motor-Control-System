/*
 * motor.h
 *
 *  Created on: Oct 15, 2026
 *      Author: aymen
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"   /* gives us TIM_HandleTypeDef, GPIO_TypeDef, etc. */

typedef struct {
    TIM_HandleTypeDef *htim;
    uint32_t      channel;
    GPIO_TypeDef *in1_port;  uint16_t in1_pin;
    GPIO_TypeDef *in2_port;  uint16_t in2_pin;
    int           invert;
    float         duty;      /* last commanded duty, -1..+1 */
} Motor;

void Motor_Init(Motor *m);
void Motor_Set(Motor *m, float duty);
void Motor_Coast(Motor *m);
void Motor_Standby(int enable_driver);

#endif /* INC_MOTOR_H_ */
