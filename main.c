//*****************************************************************************
//
// main.c
//
// Authors: Alex Long, Ismail Sarwari
// Last modified:   26/05/2023
//
//main control file for the helicopter, the program contains code to run 
//a helicopter rig. It supports the helicopter in controlling its altitude
//and yaw.
//*****************************************************************************


#include <helistate.h>
#include <pi_control.h>
#include <pwm.h>
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "circBufT.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "buttons4.h"
#include "driverlib/pin_map.h"
#include "altitude.h"
#include "yaw.h"
#include "switch.h"
#include "serial.h"
#include "uart.h"
#include "pi_control.h"

//*****************************************************************************
// Setting Globals, variables and constants
//*****************************************************************************


#define YAW_REF_PORT_BASE GPIO_PORTC_BASE
#define YAW_REF_PIN GPIO_PIN_4

int mean_val; //adc mean value from buffer
static int main_duty;
static int tail_duty;
//*****************************************************************************
//
// The interrupt handler for the for SysTick interrupt.
//
//*****************************************************************************
void
SysTickIntHandler(void)
{
    //
    // Initiate a conversion
    //
    updateButtons();

    ADCProcessorTrigger(ADC0_BASE, 3);


}

//*****************************************************************************
// Initialisation the clock (incl. SysTick), ADC, display
//*****************************************************************************
void
initClock (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
    //
    // Set up the period for the SysTick timer.  The SysTick timer period is
    // set as a function of the system clock.
    SysTickPeriodSet(SysCtlClockGet() / SAMPLE_RATE_HZ);
    //
    // Register the interrupt handler
    SysTickIntRegister(SysTickIntHandler);
    SysCtlPWMClockSet(PWM_DIVIDER_CODE);
    //
    // Enable interrupt and device
    SysTickIntEnable();
    SysTickEnable();
}

void
initDisplay (void)
{
    // intialise the Orbit OLED display
    OLEDInitialise ();
}

//*****************************************************************************
//
// Function displays on the screen - Altitude percentage, yaw, duty to main motor
//and duty to tail motor
//
//*****************************************************************************
void
displayPercentage(int alt, int yaw, int mainDuty, int tailDuty) {


    char string[17]; //Display 16 lines on display
    OrbitOledClear();

    usnprintf (string, sizeof(string), "Altitude: %4d%%", alt); //perc of altitude display
    OLEDStringDraw (string, 0, 0);

    usnprintf (string, sizeof(string), "Yaw:   %4d", yaw); //perc of altitude display
    OLEDStringDraw (string, 0, 1);

    usnprintf (string, sizeof(string), "Main Motor:%4d%%", mainDuty); //perc of altitude display
    OLEDStringDraw (string, 0, 2);

    usnprintf (string, sizeof(string), "Tail Motor:%4d%%", tailDuty); //perc of altitude display
    OLEDStringDraw (string, 0, 3);

}



//*****************************************************************************
//
// Main Function
//
//*****************************************************************************
int target = 0;
int
getmeanval (void)
{
    return mean_val;
}

void resetbuttons() {
    //Reset buttons

       SysCtlPeripheralReset (UP_BUT_PERIPH);
       SysCtlPeripheralReset (DOWN_BUT_PERIPH);
       SysCtlPeripheralReset (LEFT_BUT_PERIPH);
       SysCtlPeripheralReset (RIGHT_BUT_PERIPH);
}
void init_all() {
    init_switch();
    initButtons ();
    initClock ();
    init_altitude ();
    initDisplay ();
    init_yaw ();
    init_PWM();
    initUart();
}
int
main(void)
{
    resetbuttons();
    //initializing the program
    init_all();

    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);

    IntMasterEnable();

    setoffset(calc_meanVal());
    
    int alt_per;
    set_state(LANDED);

    while (1)
    {

        mean_val = calc_meanVal();
        alt_per = altitude_percentage(mean_val);


        if (checkButton (UP) == PUSHED) {
            target += 10;
        }
        if (checkButton (DOWN) == PUSHED) {
            target -= 10;
        }
        if (checkButton (RIGHT) == PUSHED) {
            change_yaw_target(15);
                }
        if (checkButton (LEFT) == PUSHED) {
            change_yaw_target(-15);
        }

//
        if ((check_switch() == 1)){

            if (get_state() == LANDED){
                set_state(TAKING_OFF);
//                set_tail_duty(40);
                target = 10;
            }
        }
//
        if (target < 0) {
            target = 0;
        } else if (target > 100) {
            target = 100;
        }

        if (check_switch() == 0) {
            set_state(LANDED);
            target = 0;
       }


        if ((get_state() == LANDED) && (alt_per != 0)) {
            set_state(LANDING);
            set_yaw_target(0);
            if (yawtodegree() == 0) {
                set_state(LANDED);
            }
        }


        main_duty = height_update(target, alt_per);
        if (get_state() != TAKING_OFF) {
            tail_duty = yaw_update(get_yaw_target(), yawtodegree());


        } else if (get_state() == TAKING_OFF){
            set_tail_duty(85);
        }


        displayPercentage(alt_per, yawtodegree(), main_duty, tail_duty);



        SysCtlDelay (SysCtlClockGet() / 60);
    }
}



