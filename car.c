#include "car.h"

int ticks_seen_carpet = 0;
bool stop_moving = false;

double KP = 0.000000;
double KI = 0;
double KD = -0.00452; // 0.001

#define SAMPLE_COUNT (5)

double previous_errors[SAMPLE_COUNT];
double previous_error_integral = 0;
int previous_errors_index = 0;
double last_error = 0;

double servo_angle_duty_cycle = STEER_SERVO_CENTER_DUTY;

void tick_drive_mode(void) {
		int weighted_brightness_sum = 0;
		int brightness_sum = 0;
		for(int i = 0; i < PIXEL_COUNT; i++) {
			weighted_brightness_sum += i * camera_data[i];
			brightness_sum += camera_data[i];
		}
		double bias = 0.9;
		double weighted_center = (double) weighted_brightness_sum / brightness_sum + bias;
		double error = weighted_center - CENTER_PIXEL;
		
		previous_error_integral -= previous_errors[previous_errors_index];
		previous_error_integral += error;
		previous_errors[previous_errors_index] = error;
		
		previous_errors_index++;
		if(previous_errors_index >= SAMPLE_COUNT) previous_errors_index = 0;
		/*
		servo_angle_duty_cycle = STEER_SERVO_CENTER_DUTY;
		if(error > 8) {
			servo_angle_duty_cycle = STEER_SERVO_RIGHT_DUTY;
		} else if(error < -8) {
			servo_angle_duty_cycle = STEER_SERVO_LEFT_DUTY;
		}
		*/
		double q = servo_angle_duty_cycle - STEER_SERVO_CENTER_DUTY;
		double k = 2;
		if(error > k) {
			double r = STEER_SERVO_RIGHT_DUTY;
			if(q < STEER_SERVO_CENTER_DUTY) r += 0.015;
			servo_angle_duty_cycle = map_range(error, k, 8, STEER_SERVO_CENTER_DUTY, r);
		} else if(error < -k) {
			double l = STEER_SERVO_LEFT_DUTY;
			if(q > STEER_SERVO_CENTER_DUTY) l -= 0.015;
			servo_angle_duty_cycle = map_range(error, -k, -8, STEER_SERVO_CENTER_DUTY, l);
		} else {
			servo_angle_duty_cycle = STEER_SERVO_CENTER_DUTY;
		}
		
		//servo_angle_duty_cycle = map_range(error, -8, 8, STEER_SERVO_LEFT_DUTY, STEER_SERVO_RIGHT_DUTY);
		// servo_angle_duty_cycle += KP * error + KI * previous_error_integral + KD * (error - last_error);
		servo_angle_duty_cycle = clamp_within(servo_angle_duty_cycle, STEER_SERVO_RIGHT_DUTY, STEER_SERVO_LEFT_DUTY);

		set_servo_angle(&steer_servo, servo_angle_duty_cycle);
		
		double average_bright = (double) brightness_sum / PIXEL_COUNT;
		if(average_bright < 3000) {
			ticks_seen_carpet++;
		} else {
			ticks_seen_carpet = 0;
		}
		
		if(ticks_seen_carpet > 1000) stop_moving = true;
		
		if(stop_moving) {
			set_both_drive_motor_speed(0);
		} else {
			set_both_drive_motor_speed(0.3);
		}
		
		last_error = error;
}

char bluetooth_str_buffer[64];
int bluetooth_str_index = 0;

void clear_bluetooth_string(void) {
	for(int i = 0; i < 64; i++) {
		bluetooth_str_buffer[i] = 0;
	}
	bluetooth_str_index = 0;
}

void tick_bluetooth(void) {
	char c = 0;
	while(dataAvailable_bluetooth()) {
		c = getchar_bluetooth();
		bluetooth_str_buffer[bluetooth_str_index] = c;
		bluetooth_str_index++;
	}
	
	if(c != '\n') return;
	
	double turn_angle, turn_mag;
	if(sscanf(bluetooth_str_buffer, "J1:%lf,%lf\n", &turn_angle, &turn_mag) == 2) {
		double turn_x = turn_mag * cos(turn_angle*PI/180);
		
		double angle = map_range(turn_x, -1, 1, STEER_SERVO_LEFT_DUTY, STEER_SERVO_RIGHT_DUTY);
		set_servo_angle(&steer_servo, angle);
	}
	
	double drive_angle, drive_mag;
	if(sscanf(bluetooth_str_buffer, "J0:%lf,%lf\n", &drive_angle, &drive_mag) == 2) {
		double speed_y = drive_mag * sin(drive_angle*PI/180);
		
		double speed = map_range(speed_y, -1, 1, -0.35, 0.35);
		set_both_drive_motor_speed(speed);
	}
	
	/*
	if(strcmp(bluetooth_str_buffer, "B0\n") == 0) {
		set_servo_angle(&steer_servo, steer_servo.duty_cycle + 0.001);
	}
	*/
	
	clear_bluetooth_string();
}

int main(void) {
	init_uart_serial();
	init_steer_servo();
	init_camera();
	init_OLED_display();
	init_drive_motors();
	init_buttons();
	//init_bluetooth();
	
	for(int i = 0; i < SAMPLE_COUNT; i++) {
		previous_errors[i] = 0;
	}
	
	clear_bluetooth_string();
	
	while(1) {
		//tick_bluetooth();
		tick_drive_mode();
	}
}
