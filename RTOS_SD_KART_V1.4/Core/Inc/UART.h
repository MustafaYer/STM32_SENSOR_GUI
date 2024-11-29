/*
 * UART.h
 *
 *  Created on: Oct 13, 2024
 *      Author: musta
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stm32f446xx.h"
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "string.h"

uint8_t CalculateCRC(uint8_t *data, uint16_t length);

//void SendTemperaturePacket(UART_HandleTypeDef *huart, uint8_t *data, uint16_t length);
//void SendHumidityPacket(UART_HandleTypeDef *huart, uint8_t *data, uint16_t length);

void Uart2_Config();

void UART2_SendChar(uint8_t c);

void UART2_SendString (char *string);

uint8_t UART2_GetChar (void);

void SendTemperaturePacket(uint8_t *data, uint16_t length);
void SendHumidityPacket(uint8_t *data, uint16_t length);

#endif /* INC_UART_H_ */
