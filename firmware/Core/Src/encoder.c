/*
 * encoder.c
 *
 *  Created on: Oct 15, 2026
 *      Author: aymen
 */

#include "encoder.h"
#include "config.h"

void Encoder_Init(Encoder *e)
{
    HAL_TIM_Encoder_Start(e->htim, TIM_CHANNEL_ALL);
    e->last  = (uint16_t)__HAL_TIM_GET_COUNTER(e->htim);
    e->delta = 0;
    e->total = 0;
    e->rpm   = 0.0f;
}

void Encoder_Update(Encoder *e)
{
    uint16_t now = (uint16_t)__HAL_TIM_GET_COUNTER(e->htim);
    int16_t d = (int16_t)(uint16_t)(now - e->last);
    e->last = now;
    if (e->invert == 1) d = -d;

    e->delta = d;
    e->total = e->total + d;
    e->rpm   = (float)d / ENC_CPR * 100.0f * 60.0f;
}
