#ifndef _DRIVE_MOTORS_H_
#define _DRIVE_MOTORS_H_

#include "msp.h"
#include "common.h"
#include "pwm.h"

#define MIN_DRIVE_DUTY (0.15)
#define MAX_DRIVE_DUTY (0.2)

#define DRIVE_MOTORS_TIMER (TIMER_A0)

void init_drive_motors(void);
void set_left_drive_motor_speed(double speed);
void set_right_drive_motor_speed(double speed);
void set_both_drive_motor_speed(double speed);

#endif
