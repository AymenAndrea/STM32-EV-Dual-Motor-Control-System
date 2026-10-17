/*
 * encoder.h
 *
 *  Created on: Oct 15, 2026
 *      Author: aymen
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_
#include "main.h"

typedef struct {
    TIM_HandleTypeDef *htim;   /* TIM3 or TIM4                   */
    int      invert;           /* 1 = flip direction             */
    uint16_t last;             /* counter at the previous tick   */
    int16_t  delta;            /* change in the last tick        */
    int32_t  total;            /* total counts since startup     */
    float    rpm;              /* wheel speed                    */
} Encoder;

void Encoder_Init(Encoder *e);
void Encoder_Update(Encoder *e);  /* call once every 10 ms */


#endif /* INC_ENCODER_H_ */
