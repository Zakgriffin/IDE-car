#include "button.h"

Button left_button;
Button right_button;

void new_button(Button* button, Port* port, int pin) {
	button->port = port;
	button->pin = pin;
}

void init_button(Button* button) {
	button->port->SEL0 &= ~button->pin;
	button->port->SEL1 &= ~button->pin;
	button->port->DIR |= button->pin;
	button->port->REN |= button->pin; // pull up
	button->port->OUT |= button->pin;
}

void init_buttons(void) {
	new_button(&left_button, PORT(1), BIT1);
	new_button(&right_button, PORT(1), BIT4);
	
	init_button(&left_button);
	init_button(&right_button);
}

bool check_button_pressed(Button* button) {
	return !(button->port->IN & button->pin);
}
