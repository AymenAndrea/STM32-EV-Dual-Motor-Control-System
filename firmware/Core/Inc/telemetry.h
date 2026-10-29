/*
 * telemetry.h
 *
 *  Created on: Oct 29, 2026
 *      Author: aymen
 */

#ifndef INC_TELEMETRY_H_
#define INC_TELEMETRY_H_
extern int tel_on;
#include <stdint.h>
extern int step_active;
extern uint32_t step_t0;
void Telemetry_Header(void);
void Telemetry_Line(float tempC);

#endif /* INC_TELEMETRY_H_ */
