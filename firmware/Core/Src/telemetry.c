/*
 * Telemetry.c
 *
 *  Created on: Oct 29, 2026
 *      Author: aymen
 */

#include "telemetry.h"
#include "board.h"
#include "safety.h"
#include <stdio.h>

int tel_on = 1;

void Telemetry_Header(void)
{
    printf("t_ms,state,faults,"
           "tgtA,rpmA,dutyA,iA_mA,"
           "tgtB,rpmB,dutyB,iB_mA,"
           "vbus,tempC\r\n");
}

/* written in H4b */
void Telemetry_Line(float tempC)
{
    printf("%lu,%d,%lu,"                       /* t_ms, state, faults      */
           "%.1f,%.1f,%.2f,%.1f,"              /* tgtA, rpmA, dutyA, iA_mA */
           "%.1f,%.1f,%.2f,%.1f,"              /* tgtB, rpmB, dutyB, iB_mA */
           "%.2f,%.1f\r\n",                    /* vbus, tempC              */
           (unsigned long)HAL_GetTick(), (int)sysState, (unsigned long)faults,
           (double)targetA, (double)encA.rpm, (double)motorA.duty, (double)inaA.current_mA,
           (double)targetB,    (double)encB.rpm, (double)motorB.duty, (double)inaB.current_mA,
           (double)inaA.bus_V, (double)tempC);
}
