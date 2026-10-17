/*
 * config.h
 *
 *  Created on: Oct 14, 2026
 *      Author: aymen
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_
/* ---------------- Bring-up stage selector ----------------
 * 1 = UART hello + LED          6 = INA219 current/voltage
 * 2 = open-loop PWM, no encoder  7 = LM35 temperature
 * 3 = encoder counts (by hand)  8 = closed loop, motor A only
 * 4 = open-loop duty -> RPM     9 = FULL APPLICATION
 * 5 = I2C bus scan
 */
#define STAGE 3



/* MOTOR*/
#define MOTOR_A_INVERT 0
#define MOTOR_B_INVERT 0
#define MAX_DUTY 1.0f

/* ENCODER*/
#define ENC_CPR 4200.0f
#define CTRL_DT_S 0.01f
#define ENC_A_INVERT 0
#define ENC_B_INVERT 0

/* PID*/
#define PID_KP 0.008f
#define PID_KI 0.12f
#define PID_KD 0.0f
#define PID_KFF 0.003f
#define MAX_RPM_CMD 110.0f

/* INA219 current sensors */
#define INA_A_ADDR 0x40
#define INA_B_ADDR 0x41
#define INA_SHUNT_OHMS 0.1f

/* ---- LM35 temperature ---- */
#define LM35_SAMPLES 16
#define ADC_VREF_MV 3300.0f

#endif /* INC_CONFIG_H_ */
