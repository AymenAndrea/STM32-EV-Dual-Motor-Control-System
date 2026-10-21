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

    printf("command '%s', value %ld\r\n", name, (long)v);
    (void)tempC;
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
