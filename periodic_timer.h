#ifndef _PERIODIC_TIMER_H_
#define _PERIODIC_TIMER_H_

#include "msp.h"
#include "cortex_m.h"

#define TIMER32_LOAD1 (HWREG32(0x4000C000))
#define TIMER32_CONTROL1 (HWREG32(0x4000C008))
#define TIMER32_INTCLR1 (HWREG32(0x4000C00C))

#define NVIC_ISER0 (HWREG32(0xE000E100))
#define NVIC_IPR6 (HWREG32(0xE000E418))

typedef enum {
  T32DIV1 = 0x00000000,
  T32DIV16 = 0x00000004,
  T32DIV256 = 0x00000008
} Timer32Divider;

void init_periodic_timer_1(void (*task)(void), unsigned long rollover, Timer32Divider div);

#endif
