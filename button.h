#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <stdbool.h>
#include "common.h"

typedef struct {
	Port* port;
	int pin;
} Button;

extern Button left_button;
extern Button right_button;

void init_buttons(void);
bool check_button_pressed(Button* button);

#endif
