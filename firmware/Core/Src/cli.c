/*
 * cli.c
 *
 *  Created on: Oct 19, 2026
 *      Author: aymen
 */

#include "cli.h"
#include "board.h"
#include "config.h"
#include "safety.h"
#include "telemetry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RX_BUF 64

static volatile uint8_t rx_buf[RX_BUF];
static volatile uint8_t rx_head = 0, rx_tail = 0;
static uint8_t rx_byte;

void CLI_Init(void)
{
    HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance != USART2) return;
    uint8_t next = (uint8_t)((rx_head + 1) % RX_BUF);
    if (next != rx_tail) {
        rx_buf[rx_head] = rx_byte;
        rx_head = next;
    }
    HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    (void)huart;
}

#define LINE_MAX 40
static char    line[LINE_MAX];
static uint8_t line_len = 0;

static void run(char *cmd, float tempC);

static void run(char *cmd, float tempC)
{
    char *name = strtok(cmd, " ");
    char *arg  = strtok(NULL, " ");
    if (name == NULL) return;

    float v = 0.0f;
    if (arg != NULL) v = strtof(arg, NULL);

    /* motor commands are refused while in FAULT */
    if (sysState == STATE_FAULT && strcmp(name, "clear") != 0
                                && strcmp(name, "status") != 0
                                && strcmp(name, "help") != 0) {
            printf("In FAULT - type clear first\r\n");
            return;
        }

        /* limit the speed someone can type */
        if (v >  MAX_RPM_CMD) v =  MAX_RPM_CMD;
        if (v < -MAX_RPM_CMD) v = -MAX_RPM_CMD;

        if (strcmp(name, "a") == 0) {
            targetA = v;
        } else if (strcmp(name, "b") == 0) {
            targetB = v;
        } else if (strcmp(name, "stop") == 0) {
            targetA = 0.0f;
            targetB = 0.0f;
        } else if (strcmp(name, "kp") == 0 && arg != NULL) {
            pidA.kp = v;   pidB.kp = v;
        } else if (strcmp(name, "ki") == 0 && arg != NULL) {
            pidA.ki = v;   pidB.ki = v;
        } else if (strcmp(name, "kff") == 0 && arg != NULL) {
            pidA.kff = v; pidB.kff = v;
        } else if (strcmp(name, "help") == 0) {
            printf("a <rpm>     motor A target     e.g. a 60\r\n"
                   "b <rpm>     motor B target     e.g. b -40\r\n"
                   "stop        both targets to 0\r\n"
                   "kp|ki|kff <value>   change a gain   e.g. kp 0.01\r\n"
                   "clear       clear latched faults\r\n"
                   "status      print state, faults, rpm\r\n");
            return;
        } else if (strcmp(name, "tel") == 0 && arg != NULL) {
            if (strcmp(arg, "on") == 0) {
                tel_on = 1;
                Telemetry_Header();
            } else {
                tel_on = 0;
            }
        } else if (strcmp(name, "step") == 0) {
            step_t0 = HAL_GetTick();
            step_active = 1;
            tel_on = 1;
            Telemetry_Header();
            printf("# step test: 0 -> 60 -> 0 rpm\r\n");
        } else if (strcmp(name, "clear") == 0) {
            Safety_Clear(tempC);
            return;
        } else if (strcmp(name, "status") == 0) {
            printf("state %d | faults %lu | A %ld rpm | B %ld rpm\r\n",
                   (int)sysState, (unsigned long)faults,
                   (long)encA.rpm, (long)encB.rpm);
            return;
        } else {
            printf("? unknown: %s\r\n", name);
            return;
        }
        printf("ok\r\n");
}


void CLI_Poll(float tempC)
{

    if (huart2.RxState == HAL_UART_STATE_READY) {
        HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
    }

    while (rx_tail != rx_head) {
        char c = (char)rx_buf[rx_tail];
        rx_tail = (uint8_t)((rx_tail + 1) % RX_BUF);

        if (c == '\r' || c == '\n') {
            if (line_len > 0) {
                line[line_len] = '\0';
                printf("\r\n");
                run(line, tempC);
                line_len = 0;
            }
        } else if (line_len < LINE_MAX - 1) {
            line[line_len] = c;
            line_len++;
            putchar(c);
        }
    }
}
