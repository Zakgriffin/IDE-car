#include "drive_motors.h"

PWM_Device left_motor_forward, left_motor_backward, right_motor_forward, right_motor_backward;

void init_DC_motor(PWM_Device* motor_forward, PWM_Device* motor_backward, Port* enable_port, int enable_pin) {
	enable_port->SEL0 &= ~enable_pin;
	enable_port->SEL1 &= ~enable_pin;
	enable_port->DIR |= enable_pin;
	enable_port->OUT &= ~enable_pin;
	
	init_PWM(motor_forward);
	init_PWM(motor_backward);
	
	enable_port->OUT |= enable_pin;
	enable_port->OUT |= enable_pin;
}

void init_drive_motors(void) {
	set_PWM_rollover(DRIVE_MOTORS_TIMER, hz_to_rollover(10000));
	
	make_default_PWM_device(&left_motor_backward, PORT(2), BIT4, DRIVE_MOTORS_TIMER, 1, 0);
	make_default_PWM_device(&left_motor_forward, PORT(2), BIT5, DRIVE_MOTORS_TIMER, 2, 0);
	make_default_PWM_device(&right_motor_backward, PORT(2), BIT6, DRIVE_MOTORS_TIMER, 3, 0);
	make_default_PWM_device(&right_motor_forward, PORT(2), BIT7, DRIVE_MOTORS_TIMER, 4, 0);
	
	init_DC_motor(&left_motor_forward, &left_motor_backward, PORT(3), BIT6);
	init_DC_motor(&right_motor_forward, &right_motor_backward, PORT(3), BIT7);
}

void set_drive_motor_pair(PWM_Device* motor_forward, PWM_Device* motor_backward, double speed) {
	motor_forward->duty_cycle = speed > 0 ? speed : 0;
	update_PWM_duty_cycle(motor_forward);
	motor_backward->duty_cycle = speed < 0 ? -speed : 0;
	update_PWM_duty_cycle(motor_backward);
}

void set_left_drive_motor_speed(double speed) {
	set_drive_motor_pair(&left_motor_forward, &left_motor_backward, speed);
}

void set_right_drive_motor_speed(double speed) {
	set_drive_motor_pair(&right_motor_forward, &right_motor_backward, speed);
}

void set_both_drive_motor_speed(double speed) {
	set_left_drive_motor_speed(speed);
	set_right_drive_motor_speed(speed);
}
