/*
 * safety.c
 *
 *  Created on: Oct 18, 2026
 *      Author: aymen
 */

#include "safety.h"
#include "board.h"
#include "config.h"
#include <stdio.h>
#include <math.h>

volatile SysState sysState = STATE_NORMAL;
volatile uint32_t faults = 0;

void Safety_Trip(uint32_t flag)
{
    if (faults & flag) return;
    faults |= flag;
    sysState = STATE_FAULT;

    Motor_Standby(0);
    targetA = 0.0f;
    targetB = 0.0f;
    Motor_Coast(&motorA);
    Motor_Coast(&motorB);
    PID_Reset(&pidA, 0.0f);
    PID_Reset(&pidB, 0.0f);

    printf("\r\n!! FAULT %lu\r\n", (unsigned long)flag);
}

void Safety_Check(float tempC)
{
    static uint32_t ocA_ms = 0, ocB_ms = 0;   /* overcurrent timers */
    static uint32_t stallA_ms = 0, stallB_ms = 0;   /* stall timers */
    /* ---- overcurrent, motor A ---- */
    if (fabsf(inaA.current_mA) > OC_LIMIT_MA) {
        ocA_ms += SENSOR_PERIOD_MS;
    } else {
        ocA_ms = 0;
    }
    if (ocA_ms >= OC_TRIP_MS) Safety_Trip(F_OC_A);

    /* ---- overcurrent, motor B ---- */
    if (fabsf(inaB.current_mA) > OC_LIMIT_MA) {
        ocB_ms += SENSOR_PERIOD_MS;
    } else {
        ocB_ms = 0;
    }
    if (ocB_ms >= OC_TRIP_MS) Safety_Trip(F_OC_B);
    /* ---- stall, motor A: pushing hard AND barely turning ---- */
    if (fabsf(motorA.duty) >= STALL_DUTY && fabsf(encA.rpm) < STALL_RPM) {
        stallA_ms += SENSOR_PERIOD_MS;
    } else {
        stallA_ms = 0;
    }
    if (stallA_ms >= STALL_TRIP_MS) Safety_Trip(F_STALL_A);

    /* ---- stall, motor B ---- */
    if (fabsf(motorB.duty) >= STALL_DUTY && fabsf(encB.rpm) < STALL_RPM) {
        stallB_ms += SENSOR_PERIOD_MS;
    } else {
        stallB_ms = 0;
    }
    if (stallB_ms >= STALL_TRIP_MS) Safety_Trip(F_STALL_B);


    /* ---- sensors not answering ---- */
    if (inaA.fail_count >= I2C_FAIL_LIMIT) Safety_Trip(F_SENSOR);
    if (inaB.fail_count >= I2C_FAIL_LIMIT) Safety_Trip(F_SENSOR);

    /* ---- temperature ---- */
    if (tempC > OT_LIMIT_C)       Safety_Trip(F_OVERTEMP);      /* too hot      */
    if (tempC < TEMP_MIN_VALID_C)    Safety_Trip(F_SENSOR);      /* LM35 broken  */

    /* ---- overtemp is the only fault that clears itself (once cooled) ---- */
    if (faults == F_OVERTEMP && tempC < OT_CLEAR_C) Safety_Clear(tempC);
}

/* Turn all faults off. Called by you (later: the "clear" command),
 * or automatically when an overtemp has cooled down. */
void Safety_Clear(float tempC)
{
    if (tempC > OT_CLEAR_C) {
        printf("Still too hot - not clearing\r\n");
        return;
    }
    faults = 0;
    targetA = 0.0f;
    targetB = 0.0f;
    inaA.fail_count = 0;
    inaB.fail_count = 0;
    PID_Reset(&pidA, encA.rpm);
    PID_Reset(&pidB, encB.rpm);
    sysState = STATE_NORMAL;
    Motor_Standby(1);
    printf("Faults cleared\r\n");
}
