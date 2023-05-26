
// *******************************************************
// Code to handle the serial communication (not implemented in project)
// *******************************************************
#include <helistate.h>
#include <pwm.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "utils/ustdlib.h"
#include "altitude.h"
#include "yaw.h"
#include "main.h"
///
#include "serial.h"



#define BAUD_RATE           9600

#define UART_BASE           UART0_BASE
#define UART_PERIPH_UART    SYSCTL_PERIPH_UART0
#define UART_PERIPH_GPIO    SYSCTL_PERIPH_GPIOA
#define UART_GPIO_BASE      GPIO_PORTA_BASE
#define UART_GPIO_PIN_RX    GPIO_PIN_0
#define UART_GPIO_PIN_TX    GPIO_PIN_1
#define UART_GPIO_PINS      UART_GPIO_PIN_RX | UART_GPIO_PIN_TX

#define UART_WORD_LEN       UART_CONFIG_WLEN_8
#define UART_STOP_BIT       UART_CONFIG_STOP_ONE
#define UART_PAR_BIT        UART_CONFIG_PAR_NONE
#define UART_CONFIG         UART_WORD_LEN | UART_STOP_BIT | UART_PAR_BIT
#define STR_LEN             18

void initUart (void) {
    SysCtlPeripheralEnable(UART_PERIPH_UART);
    SysCtlPeripheralEnable(UART_PERIPH_GPIO);

    GPIOPinTypeUART(UART_GPIO_BASE, UART_GPIO_PINS);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    UARTConfigSetExpClk(UART_BASE, SysCtlClockGet(), BAUD_RATE, UART_CONFIG);

    // Enable Tx and Rx buffers and the UART module itself.
    UARTFIFOEnable(UART_BASE);
    UARTEnable(UART_BASE);
}

void
uart_send(char *string)
{
    while(*string) {
        UARTCharPut(UART_BASE, *string);
        string++;
    }
}

void
send_serial_update(int alt, int target_alt, char* strii)
{
    //char line[STR_LEN + 1];

    char line[100];

    usnprintf(line, sizeof(line),
              "Alt: %4d [%4d]\n", alt, target_alt);
    uart_send(line);

    usnprintf(line, sizeof(line),
              "Yaw: %4d [%4d]\r\n", yawtodegree(), get_yaw_target());
    uart_send(line);

//    usnprintf(line, sizeof(line), "Main: %4d%%\r\n", get_main_PWM());
//    uart_send(line);
    usnprintf(line, sizeof(line), "Main: %4d%%\r\n", getmeanval());
        uart_send(line);

    usnprintf(line, sizeof(line), "Tail: %4d%%\r\n", get_tail_PWM());
    uart_send(line);

    usnprintf(line, sizeof(line), "%16s\r\n", strii);

    uart_send(line);
}
