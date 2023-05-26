//*****************************************************************************
//
// Main flight controls
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
// Function to change altitude of the helicopter. PI controller
//
//*****************************************************************************

int
height_update(int target_alt, int current_alt)
{
    //float alt_error = target_alt - current_alt;

    double alt_error = target_alt - current_alt;
    double alt_control = (ALT_KP * alt_error) + (ALT_KI * (altitude_error_integrated + alt_error));

    //float alt_control = (ALT_KP * alt_error) + (ALT_KI * (altitude_error_integrated + alt_error));

    if (alt_control > MAX_DUTY_MAIN) {
        alt_control = MAX_DUTY_MAIN;
    } else if (alt_control < MIN_DUTY_MAIN) {
        alt_control = MIN_DUTY_MAIN;
    } else {
        altitude_error_integrated += alt_error;
    }


    set_main_duty(alt_control);
    return alt_control;
}
//*****************************************************************************
//
// Function to change yaw of the helicopter. Pi controller
//
//*****************************************************************************
int
yaw_update(int target_yaw, int current_yaw)
{
    double yaw_error = target_yaw - current_yaw +17;
    if (yaw_error < - 180){
        yaw_error += 360;
    }

//main controls

    float yaw_control = (YAW_KP * yaw_error); //+ (YAW_KI * (yaw_error_integrated + yaw_error));

    if (yaw_control > MAX_DUTY_TAIL) {
        yaw_control = MAX_DUTY_TAIL;
    } else if (yaw_control < MIN_DUTY_TAIL) {
        yaw_control = MIN_DUTY_TAIL;
    } else {
        yaw_error_integrated += yaw_error;
    }
    set_tail_duty(yaw_control);

    return yaw_control;
}
