/*
 * config.h
 *
 *  Created on: Oct 14, 2026
 *      Author: aymen
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_
/* ---------------- Bring-up stage selector ----------------
 * 1 = UART + LED (alive counter)
 * 2 = motors + encoders, open loop test sequence
 * 3 = closed-loop PID + INA219 + LM35
 * 4 = stage 3 + fault detection + state machine
 */
#define STAGE 4



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

/* LM35 temperature*/
#define LM35_SAMPLES 16
#define ADC_VREF_MV 3300.0f

/* Faults*/
#define SENSOR_PERIOD_MS 20u
#define OC_LIMIT_MA 400.0f
#define OC_TRIP_MS 100u
#define OT_LIMIT_C 50.0f
#define OT_CLEAR_C 45.0f
#define STALL_DUTY 0.6f
#define STALL_RPM 3.0f
#define STALL_TRIP_MS 500u
#define I2C_FAIL_LIMIT 5u
#define TEMP_MIN_VALID_C 5.0f

#endif /* INC_CONFIG_H_ */
