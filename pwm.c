#include "pwm.h"

void make_default_PWM_device(PWM_Device *pwm_device, Port* port, int pin, Timer_A_Type* timer, int timer_index, double initial_duty_cycle) {
	pwm_device->port = port;
	pwm_device->pin = pin;
	pwm_device->timer = timer;
	pwm_device->timer_index = timer_index;
	
	pwm_device->prescaler = 1;
	pwm_device->duty_cycle = initial_duty_cycle;
}

void set_PWM_rollover(Timer_A_Type *timer, unsigned long rollover) { timer->CCR[0] = rollover; }

void update_PWM_duty_cycle(PWM_Device *pwm_device) {
  pwm_device->timer->CCR[pwm_device->timer_index] = pwm_device->timer->CCR[0] * pwm_device->duty_cycle;
}

void init_PWM(PWM_Device *pwm_device) {
	pwm_device->port->SEL0 |= pwm_device->pin;
	pwm_device->port->SEL1 &= ~pwm_device->pin;
	pwm_device->port->DIR |= pwm_device->pin;
	
	pwm_device->timer->EX0 |= pwm_device->prescaler - 1;
  pwm_device->timer->CCTL[pwm_device->timer_index] = TIMER_A_CCTLN_OUTMOD_7;
	
	update_PWM_duty_cycle(pwm_device);
	
	pwm_device->timer->CTL |= TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID_0 | TIMER_A_CTL_MC__UP | TIMER_A_CTL_IE | BIT7;
	// ZZZ need BIT7 for servo only??
}
