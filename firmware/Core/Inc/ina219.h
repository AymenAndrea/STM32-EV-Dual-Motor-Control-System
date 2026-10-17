/*
 * ina219.h
 *
 *  Created on: Oct 16, 2026
 *      Author: aymen
 */

#ifndef INC_INA219_H_
#define INC_INA219_H_

#include "main.h"

typedef struct {
    I2C_HandleTypeDef *hi2c;
    uint8_t addr;
    float current_mA;
    float bus_V;
    uint32_t fail_count;
} INA219;

HAL_StatusTypeDef INA219_Init(INA219 *d);
HAL_StatusTypeDef INA219_Read(INA219 *d);

#endif
