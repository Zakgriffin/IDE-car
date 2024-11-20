#ifndef _UART_SERIAL_H_
#define _UART_SERIAL_H_

#include <stdio.h>
#include "msp.h"

#define BAUD_RATE (9600)

#define print(...) { \
	char msg[128]; \
	sprintf(msg, __VA_ARGS__); \
	print_string(msg); \
}

void init_uart_serial(void);
void print_string(char *ptr_str);
void print_char(char ch);

#endif
