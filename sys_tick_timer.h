#ifndef _SYS_TICK_TIMER_H_
#define _SYS_TICK_TIMER_H_

#include "msp.h"
#include "cortex_m.h"

#define SYSTICK_STCSR (HWREG32(0xE000E010))
#define SYSTICK_STRVR (HWREG32(0xE000E014))
#define SYSTICK_STCVR (HWREG32(0xE000E018))
#define SCB_SHPR3 (HWREG32(0xE000ED20))

extern long sys_tick_counter;

void init_sys_tick_timer(void (*task)(void), unsigned long period);

void enable_sys_tick_timer(void);
void disable_sys_tick_timer(void);

#endif
