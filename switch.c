//*****************************************************************************
//
// Functions for handling the switch for the helicopter and initialization
//
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "switch.h"

#define SWITCH_1_PERIPH SYSCTL_PERIPH_GPIOA
#define SWITCH_1_PORT_BASE GPIO_PORTA_BASE
#define SWITCH_1_PIN GPIO_PIN_7

bool switch_position;
bool pos_changed = false;

//initialize the switch
void init_switch(void)
{
    SysCtlPeripheralEnable(SWITCH_1_PERIPH);
    GPIOPinTypeGPIOInput(SWITCH_1_PORT_BASE, SWITCH_1_PIN);
    GPIOPadConfigSet(SWITCH_1_PORT_BASE, SWITCH_1_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

    switch_position = GPIOPinRead(SWITCH_1_PORT_BASE, SWITCH_1_PIN) == SWITCH_1_PIN;
}

//returns the current switch value
int check_switch (void)
{
    int switch_val = GPIOPinRead(SWITCH_1_PORT_BASE, SWITCH_1_PIN);
    int state = 0;
    if (switch_val > 0)
    {
        state = 1;
    }
    return state;
}
