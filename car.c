#include "car.h"

void test_pieces(void) {
	//print("WOW!\r\n");

/*
	for(double d = 1.1/20; d < 1.9/20; d += 0.001) {
		set_steer_servo_angle(d);
		for(int i = 0; i < 100000; i++);
	}
*/
	
	plot_line_data(camera_data);

	/*
	for(int i = 0; i < 128; i++) {
		print("%d ", camera_data[i]);
		print("\r\n");
	}
	*/
}

int slow_down_counter = 10000;

bool captured = false;
void tick_capture_mode(void) {
	if(dataAvailable_bluetooth()){
			char character = getchar_bluetooth();
			if(character == 'g'){
				set_both_drive_motor_speed(0.2);
			}
			else if(character == 's'){
				set_both_drive_motor_speed(0);
			}
	 }
	
	
	bool left_pressed = check_button_pressed(&left_button);
	bool right_pressed = check_button_pressed(&right_button);
	
	plot_line_data(camera_data);
	
	if(!captured && left_pressed && right_pressed) {
		print_bluetooth("servo: %f\r\n", steer_servo.duty_cycle);
		print_bluetooth("camera: [");
		for(int i = 0 ; i < 128; i++) {
			print_bluetooth("%d, ", camera_data[i]);
		}
		print_bluetooth("]\r\n\r\n");
		
		captured = true;
	} else {
		if(!left_pressed && !right_pressed) captured = false;
		
		double nudge = 0.001;
		if(left_pressed) {
			set_servo_angle(&steer_servo, clamp_within(steer_servo.duty_cycle + nudge, STEER_SERVO_LEFT_DUTY, STEER_SERVO_RIGHT_DUTY));
		}
		
		if(right_pressed) {
			set_servo_angle(&steer_servo, clamp_within(steer_servo.duty_cycle - nudge, STEER_SERVO_LEFT_DUTY, STEER_SERVO_RIGHT_DUTY));
		}
	}
}

int ticks_seen_carpet = 0;
bool stop_moving = false;

double integral = 0;

double Kp = 15;
double Ki = 0;
double Kd = -3;
double last_error = 0;

void tick_drive_mode(void) {
	int mass_sum = 0;
	int weight_mass_sum = 0;
	int bright_sum = 0;
	for(int i = 0; i < 128; i++) {
		weight_mass_sum += i * camera_data[i];
		mass_sum += camera_data[i];
		bright_sum += camera_data[i];
	}
	double center_of_mass = (double) weight_mass_sum / mass_sum + 3;
	
	double error = center_of_mass - 64;
	integral += error;
	
	double rolling_center = Kp * error + Ki * integral + Kd * (error - last_error);
	rolling_center = clamp_within(rolling_center, -64, 64);
	/*
	print("rolling_center: %f\r\n", rolling_center);
	print("error: %f\r\n", error);
	print("last_error: %f\r\n", last_error);
	print("integral: %f\r\n", integral);
	*/
	double average_bright = bright_sum / 128.0;
	
	double center_expected_limit = 50;
	
	double turning = map_range(rolling_center, -64, 64, -1, 1);
	double duty_cycle = map_range(turning, -1, 1, STEER_SERVO_RIGHT_DUTY, STEER_SERVO_LEFT_DUTY);
	duty_cycle = clamp_within(duty_cycle, STEER_SERVO_LEFT_DUTY, STEER_SERVO_RIGHT_DUTY);
	set_servo_angle(&steer_servo, duty_cycle);
	
	double turning_gain = clamp_within(1-double_abs(turning), 0, 1);
	double speed = map_range(turning_gain, 0, 1, MIN_DRIVE_DUTY, MAX_DRIVE_DUTY);
	
	if(average_bright < 3000) {
		ticks_seen_carpet++;
	} else {
		ticks_seen_carpet = 0;
	}
	
	if(ticks_seen_carpet > 1000) {
		stop_moving = true;
	}
	
	if(stop_moving) {
		set_both_drive_motor_speed(0);
	} else {
		set_both_drive_motor_speed(speed);
	}
	
	last_error = error;
}

int main(void) {
	init_uart_serial();
	init_steer_servo();
	init_camera();
	init_OLED_display();
	init_drive_motors();
	init_buttons();
	init_bluetooth();
	
	while(1) {
		tick_drive_mode();
	}
}
