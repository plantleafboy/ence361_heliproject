//*****************************************************************************
//
//functions to handle and support the controls for the helicopter. Uses PI control
//
//*****************************************************************************

#include <pi_control.h>
#include <pwm.h>
#include <stdint.h>
#include <stdbool.h>


#define ALT_KP 5
#define ALT_KI 0.1
#define YAW_KP 2.5
#define YAW_KI 0.1
#define MAX_DUTY_MAIN 98
#define MIN_DUTY_MAIN 2
#define MAX_DUTY_TAIL 85
#define MIN_DUTY_TAIL 2

static int altitude_error_integrated = 0;
static int yaw_error_integrated = 0;

//*****************************************************************************
//
// Function to change the helicopters yaw. Takes a target input and 
//uses PI control to reach the target. drives the tail motor by a calculated value
//
//*****************************************************************************

int
height_update(int target_alt, int current_alt)
{

    double alt_error = target_alt - current_alt;
    //Uses PI to handle the error and sets the alt_control
    double alt_control = (ALT_KP * alt_error) + (ALT_KI * (altitude_error_integrated + alt_error));

    //Clips the alt_control value if it exceeds the max duty value 
    if (alt_control > MAX_DUTY_MAIN) {
        alt_control = MAX_DUTY_MAIN;
    } else if (alt_control < MIN_DUTY_MAIN) {
        alt_control = MIN_DUTY_MAIN;
    } else {
        altitude_error_integrated += alt_error;
    }


    set_main_duty(alt_control);
    //return the value of the alt_control to the main program 
    return alt_control;
}
//*****************************************************************************
//
// Function to change the helicopters yaw. Takes a target input and 
//uses PI control to reach the target. drives the tail motor by a calculated value
//
//*****************************************************************************
int
yaw_update(int target_yaw, int current_yaw)
{
    double yaw_error = target_yaw - current_yaw +17;
    if (yaw_error < - 180){
        yaw_error += 360;
    }
    //Uses a proportional gain to handle the error and sets the yaw_control
    float yaw_control = (YAW_KP * yaw_error);

    //Clips the yaw_control value if it exceeds the max duty value 
    if (yaw_control > MAX_DUTY_TAIL) {
        yaw_control = MAX_DUTY_TAIL;
    } else if (yaw_control < MIN_DUTY_TAIL) {
        yaw_control = MIN_DUTY_TAIL;
    } else {
        yaw_error_integrated += yaw_error;
    }
    set_tail_duty(yaw_control);

    //return yaw_duty to the main program to be displayed
    return yaw_control;
}
