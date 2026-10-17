/*
 * ina219.c
 *
 *  Created on: Oct 16, 2026
 *      Author: aymen
 */

#include "ina219.h"
#include "config.h"


#define REG_CONFIG  0x00
#define REG_SHUNT   0x01
#define REG_BUS     0x02


#define INA_CONFIG  0x19E7


static HAL_StatusTypeDef wr16(INA219 *d, uint8_t reg, uint16_t v)
{
    uint8_t b[2] = { (uint8_t)(v >> 8), (uint8_t)v };
    return HAL_I2C_Mem_Write(d->hi2c, (uint16_t)(d->addr << 1), reg,
                             I2C_MEMADD_SIZE_8BIT, b, 2, 10);
}


static HAL_StatusTypeDef rd16(INA219 *d, uint8_t reg, uint16_t *v)
{
    uint8_t b[2];
    HAL_StatusTypeDef st = HAL_I2C_Mem_Read(d->hi2c, (uint16_t)(d->addr << 1),
                                            reg, I2C_MEMADD_SIZE_8BIT, b, 2, 10);
    if (st == HAL_OK) *v = (uint16_t)((b[0] << 8) | b[1]);
    return st;
}

HAL_StatusTypeDef INA219_Init(INA219 *d)
{
    d->current_mA = 0.0f;
    d->bus_V      = 0.0f;
    d->fail_count = 0;
    return wr16(d, REG_CONFIG, INA_CONFIG);
}

HAL_StatusTypeDef INA219_Read(INA219 *d)
{
    uint16_t shunt, bus;
    if (rd16(d, REG_SHUNT, &shunt) != HAL_OK || rd16(d, REG_BUS, &bus) != HAL_OK) {
        d->fail_count++;
        return HAL_ERROR;
    }
    d->fail_count = 0;

    float shunt_uV = (float)(int16_t)shunt * 10.0f;
    d->current_mA  = shunt_uV / (INA_SHUNT_OHMS * 1000.0f);
    d->bus_V       = (float)(bus >> 3) * 0.004f;
    return HAL_OK;
}
