/*
 * uart.c
 *
 *  Created on: 18/05/2023
 *      Author: isa50
 */




//********************************************************
//
// uartDemo.c - Example code for ENCE361
//
// Link with modules:  buttons2, OrbitOLEDInterface
//
// Author:  P.J. Bones  UCECE
// Last modified:   16.4.2018
//

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "utils/ustdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "buttons4.h"
#include "uart.h"
static int8_t upPushes = 0;
static int8_t downPushes = 0;
//
//********************************************************
// Constants
//********************************************************
#define SYSTICK_RATE_HZ 100
#define SLOWTICK_RATE_HZ 4
#define MAX_STR_LEN 16
//---USB Serial comms: UART0, Rx:PA0 , Tx:PA1
#define BAUD_RATE 9600
#define UART_USB_BASE           UART0_BASE
#define UART_USB_PERIPH_UART    SYSCTL_PERIPH_UART0
#define UART_USB_PERIPH_GPIO    SYSCTL_PERIPH_GPIOA
#define UART_USB_GPIO_BASE      GPIO_PORTA_BASE
#define UART_USB_GPIO_PIN_RX    GPIO_PIN_0
#define UART_USB_GPIO_PIN_TX    GPIO_PIN_1
#define UART_USB_GPIO_PINS      UART_USB_GPIO_PIN_RX | UART_USB_GPIO_PIN_TX
//
////********************************************************
//// Prototypes
////********************************************************
//void SysTickIntHandler (void);
//void initClock (void);
//void initSysTick (void);
//void initDisplay (void);
//void initialiseUSB_UART (void);
//void UARTSend (char *pucBuffer);
//void displayButtonState (char *butStr, char *stateStr,
//    uint8_t numPushes, uint8_t charLine);
//
////********************************************************
//// Global variables
////********************************************************
char statusStr[MAX_STR_LEN + 1];
volatile uint8_t slowTick = false;
//

////********************************************************
//// initialiseUSB_UART - 8 bits, 1 stop bit, no parity
////********************************************************
void
initialiseUSB_UART (void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    //
    SysCtlPeripheralEnable(UART_USB_PERIPH_UART);
    SysCtlPeripheralEnable(UART_USB_PERIPH_GPIO);
    //
    // Select the alternate (UART) function for these pins.
    //
    GPIOPinTypeUART(UART_USB_GPIO_BASE, UART_USB_GPIO_PINS);
    GPIOPinConfigure (GPIO_PA0_U0RX);
    GPIOPinConfigure (GPIO_PA1_U0TX);

    UARTConfigSetExpClk(UART_USB_BASE, SysCtlClockGet(), BAUD_RATE,
            UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
            UART_CONFIG_PAR_NONE);
    UARTFIFOEnable(UART_USB_BASE);
    UARTEnable(UART_USB_BASE);
}
//
//
////**********************************************************************
//// Transmit a string via UART0
////**********************************************************************
void
UARTSend (char *pucBuffer)
{
    // Loop while there are more characters to send.
    while(*pucBuffer)
    {
        // Write the next character to the UART Tx FIFO.
        UARTCharPut(UART_USB_BASE, *pucBuffer);
        pucBuffer++;
    }
}
//
////********************************************************
//// Function to display a status message on the OLED
////********************************************************
void
displayButtonState (char *butStr, char *stateStr, uint8_t numPushes, uint8_t charLine)
{
    char string[MAX_STR_LEN + 1]; //Display fits 16 characters per line.

//
//    OLEDStringDraw ("                ", 0, charLine);
//    usprintf (string, "%s - %s %d", butStr, stateStr, numPushes); // * usprintf
    upPushes ++;
    usprintf (statusStr, "%d up %d | ", upPushes, downPushes);
    UARTSend (statusStr);

}


