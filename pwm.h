#ifndef _PWM_H_
#define _PWM_H_

#include "msp.h"
#include "common.h"

typedef struct {
	Port *port;
	int pin;
	Timer_A_Type *timer;
	uint16_t timer_index;
	
	int prescaler;
	double duty_cycle;
} PWM_Device;

void make_default_PWM_device(PWM_Device *pwm_device, Port* port, int pin, Timer_A_Type* timer, int timer_index, double initial_duty_cycle);
void init_PWM(PWM_Device *pwm_device);
void set_PWM_rollover(Timer_A_Type *timer, unsigned long rollover);
void update_PWM_duty_cycle(PWM_Device *pwm_device);

#endif
