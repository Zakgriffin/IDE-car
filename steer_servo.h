#ifndef _STEER_SERVO_H_
#define _STEER_SERVO_H_

#include "pwm.h"

#define STEER_SERVO_PORT (PORT(5))
#define STEER_SERVO_PIN (BIT6)
#define STEER_SERVO_TIMER (TIMER_A2)

#define STEER_SERVO_LEFT_DUTY (1.05/20)
#define STEER_SERVO_CENTER_DUTY (1.5/20.0)
#define STEER_SERVO_RIGHT_DUTY (1.95/20)

extern PWM_Device steer_servo;

void init_steer_servo(void);
void set_servo_angle(PWM_Device* servo, double duty_cycle);

#endif
