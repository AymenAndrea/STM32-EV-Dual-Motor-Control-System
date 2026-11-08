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
#include <math.h>
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
static int regenA = 0;
static int regenB = 0;

static int regen_wanted(float target, float rpm, int in_regen){
	float margin = REGEN_ENTER_RPM;
	if (in_regen == 1){
		margin = REGEN_EXIT_RPM;
	}
	if (fabsf(rpm)<REGEN_MIN_RPM){
		return 0;
	}
	if (target * rpm <= 0.0f){
		return 1;
	}
	if (fabsf(target) < fabsf(rpm) - margin){
		return 1;
	}
	return 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6) {
        Encoder_Update(&encA);
        Encoder_Update(&encB);
        if (STAGE >= 3 && sysState != STATE_FAULT) {
        	/*Motor A*/
        	regenA = regen_wanted(targetA, encA.rpm, regenA);
        	float loA = -MAX_DUTY;
        	float hiA = MAX_DUTY;
        	if (regenA == 1){
        		if(encA.rpm > 0.0f){
        			loA = 0.0f;
        		}
        		else {
        			hiA = 0.0f;
        		}
        	}
        	if (targetA == 0.0f && regenA == 0){
        		Motor_Coast(&motorA);
        		PID_Reset(&pidA, encA.rpm);
        	} else{
        		float dutyA = PID_Update(&pidA, targetA, encA.rpm, CTRL_DT_S, loA, hiA);
        		Motor_Set(&motorA, dutyA);
        	}
        	/*Motor B*/
        	regenB = regen_wanted(targetB, encB.rpm, regenB);
        	float loB = -MAX_DUTY;
        	float hiB = MAX_DUTY;
        	if (regenB == 1){
        		if (encB.rpm > 0.0f){
        			loB = 0.0f;
        		}
        		else {
        			hiB = 0.0f;
        		}
        	}
        	if (targetB == 0.0f && regenB == 0){
        	    Motor_Coast(&motorB);
        	    PID_Reset(&pidB, encB.rpm);
        	} else{
        	    float dutyB = PID_Update(&pidB, targetB, encB.rpm, CTRL_DT_S, loB, hiB);
        	    Motor_Set(&motorB, dutyB);
        	}
        	if (regenA == 1 || regenB == 1){
        		sysState = STATE_REGEN;
        	}
        	else{
        		sysState = STATE_NORMAL;
        	}
        }
    }
}
