
//*****************************************************************************
// Module to control PWM signals for the helicopter rotors
//*****************************************************************************

#include <pwm.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"


static int prev_main_duty;
static int prev_tail_duty;

//*****************************************************************************
// Constants
//*****************************************************************************

#define PWM_DIVIDER 4
#define PWM_FREQ 200


//Main rotor
#define PWM_MAIN_BASE        PWM0_BASE
#define PWM_MAIN_GEN         PWM_GEN_3
#define PWM_MAIN_OUTNUM      PWM_OUT_7
#define PWM_MAIN_OUTBIT      PWM_OUT_7_BIT
#define PWM_MAIN_PERIPH_PWM  SYSCTL_PERIPH_PWM0
#define PWM_MAIN_PERIPH_GPIO SYSCTL_PERIPH_GPIOC
#define PWM_MAIN_GPIO_BASE   GPIO_PORTC_BASE
#define PWM_MAIN_GPIO_CONFIG GPIO_PC5_M0PWM7
#define PWM_MAIN_GPIO_PIN    GPIO_PIN_5

//Tail Rotor
#define PWM_TAIL_BASE        PWM1_BASE
#define PWM_TAIL_GEN         PWM_GEN_2
#define PWM_TAIL_OUTNUM      PWM_OUT_5
#define PWM_TAIL_OUTBIT      PWM_OUT_5_BIT
#define PWM_TAIL_PERIPH_PWM  SYSCTL_PERIPH_PWM1
#define PWM_TAIL_PERIPH_GPIO SYSCTL_PERIPH_GPIOF
#define PWM_TAIL_GPIO_BASE   GPIO_PORTF_BASE
#define PWM_TAIL_GPIO_CONFIG GPIO_PF1_M1PWM5
#define PWM_TAIL_GPIO_PIN    GPIO_PIN_1

void
init_PWM(void)
{
    //    SysCtlPWMClockSet(PWM_DIVIDER_CODE);
    //    SysCtlPeripheralReset(PWM_MAIN_PERIPH_GPIO);
    //    SysCtlPeripheralReset(PWM_MAIN_PERIPH_PWM);
    //    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWM);
    //    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO);
    //    GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);
    //    GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
    //
    //    uint32_t ui32Period = SysCtlClockGet() / PWM_DIVIDER / PWM_RATE_HZ;
    //
    //    PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN,
    //    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    //    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, ui32Period);
    //    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM, ui32Period * PWM_START_PC / 100);
    //    PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);

        //    main rotor
        SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWM);
        SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO);

        GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
        GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);

        PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN,
                        PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
        // Set the initial PWM parameters
//        setPWMMain (PWM_RATE_HZ, PWM_START_PC);

        PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);
        PWMGenEnable(PWM_MAIN_BASE, PWM_TAIL_GEN);

        // Disable the output.  Repeat this call with 'true' to turn O/P on.
        PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);

        //  tail rotor
        SysCtlPeripheralEnable(PWM_TAIL_PERIPH_PWM);
        SysCtlPeripheralEnable(PWM_TAIL_PERIPH_GPIO);

        GPIOPinConfigure(PWM_TAIL_GPIO_CONFIG);
        GPIOPinTypePWM(PWM_TAIL_GPIO_BASE, PWM_TAIL_GPIO_PIN);
        PWMGenConfigure(PWM_TAIL_BASE, PWM_TAIL_GEN,
                        PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

//        setPWMTail(PWM_RATE_HZ, PWM_START_PC);

        PWMGenEnable(PWM_TAIL_BASE, PWM_TAIL_GEN);

        PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, false);
}

void
set_main_duty (uint32_t duty)
{
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period = SysCtlClockGet() / PWM_DIVIDER / PWM_FREQ;

    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, ui32Period);
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM,
        ui32Period * duty / 100);
    prev_main_duty = duty;
}

void
set_tail_duty (uint32_t duty)
{
    // Calculate the PWM period corresponding to the freq.
    uint32_t ui32Period = SysCtlClockGet() / PWM_DIVIDER / PWM_FREQ;

    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, ui32Period);
    PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM,
        ui32Period * duty / 100);
    prev_tail_duty = duty;
}



void
stop_main_PWM (void)
{
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);
}

void
stop_tail_PWM (void)
{
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, false);
}

int
get_tail_PWM (void)
{
    return prev_tail_duty;
}

int
get_main_PWM (void)
{
    return prev_main_duty;
}
int
get_main_mean (void)
{
    return prev_main_duty;
}

