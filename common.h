#ifndef _COMMON_H_
#define _COMMON_H_

#include "msp.h"

typedef unsigned char Byte;

// ZZZZ temp, this is stupid and unsafe for even timers
typedef DIO_PORT_Odd_Interruptable_Type Port;
#define PORT(n) ((Port*) (DIO_BASE + ((n-1)/2)*0x20 + (n%2 == 0)))

// ZZZZ should be unsigned long or int?
unsigned long hz_to_rollover(unsigned long frequency_hz);

double map_range(double n, double start1, double stop1, double start2, double stop2);
double clamp_within(double duty, double low, double high);
double double_abs(double d);

#endif
