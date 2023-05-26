//*****************************************************************************
//
// Functions for handling the yaw of the helicopter
//
//*****************************************************************************
#include <helistate.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "OrbitOLED/OrbitOLEDInterface.h"

#include "yaw.h"

#define YAW_REF_GPIO_PERIPH SYSCTL_PERIPH_GPIOC
#define YAW_REF_GPIO_BASE GPIO_PORTC_BASE
#define YAW_REF_PIN GPIO_PIN_4


static int yaw_target;
static int16_t current;
static int16_t previous;
static uint32_t currenta;
static uint32_t currentb;
static int16_t yaw;
static uint64_t phase;
static uint64_t prevnum;
static uint64_t degree=0;
static uint32_t prevb;


// set up yaw pins and referance pins
void init_yaw (void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOInput (GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1);
    GPIOIntRegister(GPIO_PORTB_BASE, yawIntHandler);
    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1, GPIO_BOTH_EDGES);
    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1);
    current = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1);

    SysCtlPeripheralEnable(YAW_REF_GPIO_PERIPH);
    GPIOPinTypeGPIOInput(YAW_REF_GPIO_BASE, YAW_REF_PIN);
    GPIOPadConfigSet(YAW_REF_GPIO_BASE, YAW_REF_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOIntRegister(YAW_REF_GPIO_BASE, yaw_ref_int_handler);
    GPIOIntTypeSet(YAW_REF_GPIO_BASE, YAW_REF_PIN, GPIO_FALLING_EDGE);
    GPIOIntEnable(YAW_REF_GPIO_BASE, YAW_REF_PIN);

}

// *******************************************************
// quadratic decoder to track yaw by comparing states
// *******************************************************
void yawIntHandler(void) {
    
    currenta = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0) &1;
    currentb = (GPIOPinRead(GPIO_PORTB_BASE,  GPIO_PIN_1)>>1) & 1;

      if (currenta == 0 && currentb ==0) {
          phase = 1;
      }
      if (currenta == 0 && currentb ==1) {
                phase = 2;
      }
      if (currenta == 1 && currentb ==1) {
                phase = 3;
            }
      if (currenta == 1 && currentb ==0) {
                phase = 4;
      }
      if (phase == 1) {
          if (prevnum == 4) {
              yaw ++;
          }
          if (prevnum == 2) {
              yaw --;
          }
      }
      if (phase == 2) {
               if (prevnum == 1) {
                   yaw ++;
               }
               if (prevnum == 3) {
                   yaw --;
               }
           }
      if (phase == 3) {
               if (prevnum == 2) {
                   yaw ++;
               }
               if (prevnum == 4) {
                   yaw --;
               }
           }
      if (phase == 4) {
               if (prevnum == 3) {
                   yaw ++;
               }
               if (prevnum == 1) {
                   yaw --;
               }
           }
      prevnum = phase;
      yawtodegree();
      GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
 }


void
yaw_ref_int_handler (void) {
    if (get_state() == TAKING_OFF){
        yaw_target = 0;
        yaw = 0;
        set_state(FLYING);
    }
    GPIOIntClear(YAW_REF_GPIO_BASE, YAW_REF_PIN);
}

int
yawtodegree (void)
{
    int degree = ((yaw * 360) / 448) % 360;
    if (degree < 0) {
        return degree + 360;
    } else {
        return degree;
    }
}

void set_yaw_target(int target){
    yaw_target = target;
}

int get_yaw_target(void)
{
    return yaw_target;
}

void change_yaw_target(int value)
{
    yaw_target += value;
}

