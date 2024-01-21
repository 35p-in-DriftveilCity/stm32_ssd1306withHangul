/*
 * uart.c
 *
 *  Created on: Feb 3, 2023
 *      Author: pusedu03
 */

#include "uart.h"
#include <stdio.h>

UART_HandleTypeDef *huart;

#define rxBufferMax	255
int rxBufferGp;					// get pointer (read)
int rxBufferPp;					// put pointer (write)
uint8_t rxBuffer[rxBufferMax];
uint8_t rxChar;

// uart 장치 초기화
void initUart(UART_HandleTypeDef *inHuart) {
	huart = inHuart;
	HAL_UART_Receive_IT(huart, &rxChar, 1);
}

// 문자수신 처리
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	rxBuffer[rxBufferPp++] = rxChar;
	rxBufferPp %= rxBufferMax;
	HAL_UART_Receive_IT(huart, &rxChar, 1);
}

// 버퍼에서 문자 꺼내오기
uint8_t getChar() {
	uint8_t result;
	if(rxBufferGp == rxBufferPp) return 0;
	result = rxBuffer[rxBufferGp++];
	rxBufferGp %= rxBufferMax;
	return result;
}

int _write(int file, char *p, int len) {
	HAL_UART_Transmit(huart, p, len, len);
	return len;
}
