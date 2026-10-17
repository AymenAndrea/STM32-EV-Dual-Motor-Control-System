/*
 * lm35.c
 *
 *  Created on: Oct 16, 2026
 *      Author: aymen
 */

#include "lm35.h"
#include "config.h"

float LM35_ReadC(ADC_HandleTypeDef *hadc)
{
    uint32_t sum = 0;

    for (int i = 0; i < LM35_SAMPLES; i++) {
        HAL_ADC_Start(hadc);
        if (HAL_ADC_PollForConversion(hadc, 2) != HAL_OK) {
            HAL_ADC_Stop(hadc);
            return -100.0f;
        }
        sum += HAL_ADC_GetValue(hadc);
        HAL_ADC_Stop(hadc);
    }

    float raw = (float)sum / (float)LM35_SAMPLES;
    float mv  = raw * ADC_VREF_MV / 4095.0f;
    return mv / 10.0f;
}
