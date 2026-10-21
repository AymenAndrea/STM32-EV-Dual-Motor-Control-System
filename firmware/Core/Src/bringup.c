/*
 * bringup.c
 *
 *  Created on: Oct 14, 2026
 *      Author: aymen
 */
#include "bringup.h"
#include "board.h"
#include "safety.h"
#include "config.h"
#include "cli.h"
#include "main.h"
#include <stdio.h>

void Bringup_Init(void)
{
    if (STAGE == 1) {
        printf("Stage 1: UART + LED. You should see a counter every second.\r\n");
    }
    if (STAGE == 2) {
        printf("Stage 2: motors + encoders\r\n");
        Motor_Init(&motorA);
        Motor_Init(&motorB);
        Encoder_Init(&encA);
        Encoder_Init(&encB);
        Motor_Standby(1);
        HAL_TIM_Base_Start_IT(&htim6);     /* start the 10 ms tick LAST */
    }
    if (STAGE >= 3) {
        printf("Stage %d: closed-loop PID%s\r\n", STAGE, STAGE >= 4 ? " + faults" : "");
        Motor_Init(&motorA);
        Motor_Init(&motorB);
        Encoder_Init(&encA);
        Encoder_Init(&encB);
        PID_Init(&pidA, PID_KP, PID_KI, PID_KD, PID_KFF);
        PID_Init(&pidB, PID_KP, PID_KI, PID_KD, PID_KFF);
        INA219_Init(&inaA);
        INA219_Init(&inaB);
        Motor_Standby(1);
        if (STAGE >= 4) CLI_Init();
        HAL_TIM_Base_Start_IT(&htim6);
    }
}

void Bringup_Loop(void)
{
    static uint32_t t_led = 0, t_print = 0, count = 0, t_step = 0, step = 0, t_rpm = 0, t_p3 = 0, t_sens = 0;
    static float tempC = 0.0f;
    uint32_t now = HAL_GetTick();

    uint32_t blink = 500;
    if (sysState == STATE_FAULT) blink = 100;
    if (now - t_led >= blink) {
        t_led = now;
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    }

    /* Stage 1: alive counter */
    if (STAGE == 1 && now - t_print >= 1000) {
        t_print = now;
        printf("alive %lu\r\n", (unsigned long)count);
        count++;
    }

    /* Stage 2: motor test sequence, next step every 2 s */
    if (STAGE == 2 && now - t_step >= 2000) {
        t_step = now;

        if (step == 0) {
            Motor_Set(&motorA, 0.3f);
            printf("A forward 30\r\n");
        } else if (step == 1) {
            Motor_Set(&motorA, -0.5f);
            printf("A reverse 50\r\n");
        } else if (step == 2) {
            Motor_Set(&motorA, 0.0f);
            printf("A stop\r\n");
        } else if (step == 3) {
            Motor_Set(&motorB, 0.3f);
            printf("B forward 30\r\n");
        } else if (step == 4) {
            Motor_Set(&motorB, -0.5f);
            printf("B reverse 50\r\n");
        } else if (step == 5) {
            Motor_Set(&motorB, 0.0f);
            printf("B stop\r\n");
        }

        step++;
        if (step == 6) {
            step = 0;
        }
    }

    /* Stage 2: print wheel speeds twice a second */
    if (STAGE == 2 && now - t_rpm >= 500) {
        t_rpm = now;
        printf("A: %ld rpm  B: %ld rpm\r\n",
               (long)encA.rpm, (long)encB.rpm);
    }
    /* Stage 3: change the target every 3 s: 40 -> 80 -> 0 -> repeat */
    if (STAGE >= 3 && sysState == STATE_NORMAL && now - t_step >= 3000) {
        t_step = now;

        if (step == 0) {
            targetA = 40.0f;  targetB = 40.0f;
        } else if (step == 1) {
            targetA = 80.0f;  targetB = 80.0f;
        } else if (step == 2) {
            targetA = 0.0f;   targetB = 0.0f;
        }

        step++;
        if (step == 3) {
            step = 0;
        }
    }
    /* Stage 3+: read sensors every 20 ms. Stage 4: then check for faults */
    if (STAGE >= 3 && now - t_sens >= SENSOR_PERIOD_MS) {
        t_sens = now;
        INA219_Read(&inaA);
        INA219_Read(&inaB);
        tempC = LM35_ReadC(&hadc1);
        if (STAGE >= 4) Safety_Check(tempC);
    }


    if (STAGE >= 4 && HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET) {
        Safety_Trip(F_ESTOP);
    }

    /* Stage 4: check for typed commands */
    if (STAGE >= 4) CLI_Poll(tempC);

    if (STAGE >= 3 && now - t_p3 >= 200) {
        t_p3 = now;
        printf("tgt %ld | A %ld rpm %ld%% | B %ld rpm %ld%%\r\n",
               (long)targetA,
               (long)encA.rpm, (long)(motorA.duty * 100.0f),
               (long)encB.rpm, (long)(motorB.duty * 100.0f));
        printf("I: A %ld mA  B %ld mA | batt %ld.%01ld V | temp %ld C\r\n",
               (long)inaA.current_mA,
               (long)inaB.current_mA,
               (long)inaA.bus_V,
               (long)(inaA.bus_V * 10.0f) % 10,
               (long)tempC);
        printf("state %d | faults %lu\r\n", (int)sysState, (unsigned long)faults);
    }
}
