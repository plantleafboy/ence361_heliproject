#define PWM_MIN_DUTY 2
#define PWM_DIVIDER_CODE SYSCTL_PWMDIV_4
#include <stdint.h>

void init_PWM (void);

void set_main_duty (uint32_t duty);

void set_tail_duty (uint32_t duty);


//void start_main_PWM (void);

//void start_tail_PWM (void);

void stop_main_PWM (void);

void stop_tail_PWM (void);

int get_main_PWM (void);

int get_tail_PWM (void);
