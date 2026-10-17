/*
 * board.c
 *
 *  Created on: Oct 15, 2026
 *      Author: aymen
 */

#include "board.h"
#include "config.h"

Motor motorA = { &htim1, TIM_CHANNEL_1, AIN1_GPIO_Port, AIN1_Pin,
                 AIN2_GPIO_Port, AIN2_Pin, MOTOR_A_INVERT, 0.0f };
Motor motorB = { &htim1, TIM_CHANNEL_2, BIN1_GPIO_Port, BIN1_Pin,
                 BIN2_GPIO_Port, BIN2_Pin, MOTOR_B_INVERT, 0.0f };

Encoder encA = { &htim3, ENC_A_INVERT };
Encoder encB = { &htim4, ENC_B_INVERT };

PID pidA;
PID pidB;
volatile float targetA = 0.0f;
volatile float targetB = 0.0f;

INA219 inaA = { &hi2c1, INA_A_ADDR };
INA219 inaB = { &hi2c1, INA_B_ADDR };
