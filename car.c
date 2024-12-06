#include "car.h"

int ticks_seen_carpet = 0;
bool stop_moving = false;

#define SAMPLE_COUNT (5)

double dead_zone = 1.4;
double high_thresh = 6.8;
double abrupt_turn = 0.015;
double bias = 6;

int carpet_stop_thresh = 3000;

double servo_angle_duty_cycle = STEER_SERVO_CENTER_DUTY;

void tick_drive_mode(void) {
		double weighted_brightness_sum = 0;
		double brightness_sum = 0;
		for(int i = 0; i < PIXEL_COUNT; i++) {
			weighted_brightness_sum += i * camera_data[i];
			brightness_sum += camera_data[i];
		}
		
		double weighted_center = weighted_brightness_sum / brightness_sum + bias;
		double error = weighted_center - CENTER_PIXEL;
	
		double q = servo_angle_duty_cycle - STEER_SERVO_CENTER_DUTY;
		if(error > dead_zone) {
			double r = STEER_SERVO_RIGHT_DUTY;
			if(q < STEER_SERVO_CENTER_DUTY) r += abrupt_turn;
			servo_angle_duty_cycle = map_range(error, dead_zone, high_thresh, STEER_SERVO_CENTER_DUTY, r);
		} else if(error < -dead_zone) {
			double l = STEER_SERVO_LEFT_DUTY;
			if(q > STEER_SERVO_CENTER_DUTY) l -= abrupt_turn;
			servo_angle_duty_cycle = map_range(error, -dead_zone, -high_thresh, STEER_SERVO_CENTER_DUTY, l);
		} else {
			servo_angle_duty_cycle = STEER_SERVO_CENTER_DUTY;
		}
	
		servo_angle_duty_cycle = clamp_within(servo_angle_duty_cycle, STEER_SERVO_RIGHT_DUTY, STEER_SERVO_LEFT_DUTY);

		set_servo_angle(&steer_servo, servo_angle_duty_cycle);
		
		double average_bright = (double) brightness_sum / PIXEL_COUNT;
		if(average_bright < carpet_stop_thresh) {
			ticks_seen_carpet++;
		} else {
			ticks_seen_carpet = 0;
		}
		
		if(ticks_seen_carpet > 1000) stop_moving = true;
		
		if(stop_moving) {
			set_both_drive_motor_speed(0);
		} else {
			double speed = map_range(double_abs(error), 0, 32, 0.3, 0.2);
			set_both_drive_motor_speed(0.25); // 0.26
		}	
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
	
	sscanf(bluetooth_str_buffer, "dead_zone %lf", &dead_zone);
	sscanf(bluetooth_str_buffer, "high_thresh %lf", &high_thresh);
	sscanf(bluetooth_str_buffer, "abrupt_turn %lf", &abrupt_turn);
	sscanf(bluetooth_str_buffer, "bias %lf", &bias);
	
	/*
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
	
	clear_bluetooth_string();
	
	while(1) {
		//tick_bluetooth();
		tick_drive_mode();
		
		//plot_line_data(camera_data);
	}
}
