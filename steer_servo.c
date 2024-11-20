#include "steer_servo.h"

PWM_Device steer_servo;

void init_steer_servo(void) {
	make_default_PWM_device(&steer_servo, STEER_SERVO_PORT, STEER_SERVO_PIN, STEER_SERVO_TIMER, 1, STEER_SERVO_CENTER_DUTY);
	steer_servo.prescaler = 5;
	/// ZZZ clock divider of 4?
	
	set_PWM_rollover(STEER_SERVO_TIMER, hz_to_rollover(50) / 5 / 4);
	init_PWM(&steer_servo);
}

void set_servo_angle(PWM_Device* servo, double duty_cycle) {
	servo->duty_cycle = duty_cycle;
	update_PWM_duty_cycle(servo);
}
