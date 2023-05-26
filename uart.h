/*
 * uart.h
 *
 *  Created on: 18/05/2023
 *      Author: isa50
 */


#ifndef UART_H_
#define UART_H_

void
initialiseUSB_UART (void);
void
UARTSend (char *pucBuffer);

void
displayButtonState (char *butStr, char *stateStr, uint8_t numPushes, uint8_t charLine);

#endif

