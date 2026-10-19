/*
 * safety.h
 *
 *  Created on: Oct 17, 2026
 *      Author: aymen
 */

#ifndef INC_SAFETY_H_
#define INC_SAFETY_H_
#include <stdint.h>

typedef enum { STATE_NORMAL = 0, STATE_FAULT  = 1} SysState;

/* fault bits: one bit per fault, several can be on at once */
#define F_OC_A      (1u << 0) /* overcurrent, motor A */
#define F_OC_B      (1u << 1) /* overcurrent, motor B */
#define F_OVERTEMP  (1u << 2) /* too hot */
#define F_STALL_A   (1u << 3) /* motor A stuck */
#define F_STALL_B   (1u << 4) /* motor B stuck */
#define F_SENSOR    (1u << 5) /* INA219 or LM35 not working */
#define F_ESTOP     (1u << 6) /* blue button pressed */

extern volatile SysState sysState;
extern volatile uint32_t faults;
void Safety_Trip(uint32_t flag);
void Safety_Check(float tempC);
void Safety_Clear(float tempC);
#endif /* INC_SAFETY_H_ */
