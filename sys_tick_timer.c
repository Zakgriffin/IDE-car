#include "sys_tick_timer.h"

void (*sys_tick_handler)(void);
long sys_tick_counter = 0;

void enable_sys_tick_timer(void) {
  // with core clock and interrupts
  SYSTICK_STCSR = BIT0 | BIT1 | BIT2;
}

void disable_sys_tick_timer(void) {
  // with core clock and interrupts
  SYSTICK_STCSR &= ~(BIT0 | BIT1 | BIT2);
}

void init_sys_tick_timer(void (*task)(void), unsigned long period) {
  long sr = start_critical();

  sys_tick_handler = task;

  // Control and Status Register
  // disable SysTick during setup
  SYSTICK_STCSR &= ~BIT0;

  // NOTE: The STRVR - RELOAD VALUE REGISTER)is a 24 bit value
  // reload value sets period
  // SYS_TICK_STRVR
  SYSTICK_STRVR = period - 1;

  // Current Value Register
  // any write to current clears it
  SYSTICK_STCVR = 0;

  // priority 2
  // SCB_SHPR3
  // 31-24 PRI_15 R/W 0h Priority of system handler 15.
  // 23-16 PRI_14 R/W 0h Priority of system handler 14.
  // 15-8 PRI_13 R/W 0h Priority of system handler 13.
  // 7-0 PRI_12 R/W 0h Priority of system handler 12
  SCB_SHPR3 = (SCB_SHPR3 & 0x00FFFFFF) | 0x40000000;

  // Control and Status Register
  // 31-17 RESERVED
  // 2 CLKSOURCE R 1h Clock source.
  //	0b = Not applicable
  //	1b = Core clock
  //	1 TICKINT R/W 0h
  //	0 ENABLE R/W 0h Enable SysTick counter
  //	0b (R/W) = Counter disabled

  // enable SysTick with core clock and interrupts
  SYSTICK_STCSR |= BIT0;

  end_critical(sr);
  // caller needs to enable interrupts
}

void SysTick_Handler(void) {
  sys_tick_counter++;
  (*sys_tick_handler)();
}
/// ZZZZ this is wrong ^
