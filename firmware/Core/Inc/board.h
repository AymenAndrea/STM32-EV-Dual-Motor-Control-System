/*
 * board.h
 *
 *  Created on: Oct 15, 2026
 *      Author: aymen
 */

#ifndef INC_BOARD_H_
#define INC_BOARD_H_

#include "main.h"
#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "ina219.h"
#include "lm35.h"

extern TIM_HandleTypeDef  htim1;   /* PWM: CH1 = motor A, CH2 = motor B */
extern UART_HandleTypeDef huart2;  /* USB serial to the PC */
extern TIM_HandleTypeDef htim3;    /* encoder A */
extern TIM_HandleTypeDef htim4;    /* encoder B */
extern TIM_HandleTypeDef htim6;    /* 100 Hz control tick */
extern I2C_HandleTypeDef hi2c1;
extern ADC_HandleTypeDef hadc1;

extern Motor motorA, motorB;
extern Encoder encA, encB;
extern PID pidA, pidB;
extern INA219 inaA, inaB;

extern volatile float targetA, targetB;
#endif /* INC_BOARD_H_ */

