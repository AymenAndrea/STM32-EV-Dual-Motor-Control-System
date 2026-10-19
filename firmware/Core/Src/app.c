/*
 * app.c
 *
 *  Created on: Oct 14, 2026
 *      Author: aymen
 */

#include "app.h"        // its own header: matches the promises
#include "bringup.h"    // because App_Init calls Bringup_Init
#include "config.h"     // for STAGE
#include "main.h"       // HAL functions and pin names
#include <stdio.h>      // printf
#include "board.h"
#include "safety.h"

extern UART_HandleTypeDef huart2;

int _write(int file, char *ptr, int len)
{
(void)file;
HAL_UART_Transmit(&huart2, (uint8_t *)ptr, (uint16_t)len, HAL_MAX_DELAY);
  return len;
}
void App_Init(void)
{
setvbuf(stdout, NULL, _IONBF, 0);   /* send printf output immediately */
printf("\r\n=== EV dual motor controller | stage %d ===\r\n", STAGE);
printf("SYSCLK = %lu Hz\r\n", (unsigned long)SystemCoreClock);
Bringup_Init();
}
void App_Loop(void)
{
	Bringup_Loop();
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6) {
        Encoder_Update(&encA);
        Encoder_Update(&encB);
        if (STAGE >= 3 && sysState == STATE_NORMAL) {
        	/*Motor A*/
        	if (targetA == 0.0f){
        		Motor_Coast(&motorA);
        		PID_Reset(&pidA, encA.rpm);
        	} else{
        		float dutyA = PID_Update(&pidA, targetA, encA.rpm, CTRL_DT_S, -MAX_DUTY, MAX_DUTY);
        		Motor_Set(&motorA, dutyA);
        	}
        	/*Motor B*/
        	if (targetB == 0.0f){
        	    Motor_Coast(&motorB);
        	    PID_Reset(&pidB, encB.rpm);
        	} else{
        	    float dutyB = PID_Update(&pidB, targetB, encB.rpm, CTRL_DT_S, -MAX_DUTY, MAX_DUTY);
        	    Motor_Set(&motorB, dutyB);
        	}
        }
    }
}
