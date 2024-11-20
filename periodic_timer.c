#include "periodic_timer.h"

void (*Timer32_1_PeriodicTask)(void);
unsigned long timer_1_rollover;

void init_periodic_timer_1(void (*task)(void), unsigned long rollover, Timer32Divider div) {
  timer_1_rollover = rollover;

  long sr = start_critical();

  Timer32_1_PeriodicTask = task;

  // timer reload value
  TIMER32_LOAD1 = rollover;

  // clear Timer32 Timer 1 interrupt
  TIMER32_INTCLR1 = 0;

  // bits31-8=X...X,   reserved
  // bit7,             timer 0=disable, 1=enable
  // bit6,             0 = Timer is in free-running mode, 1=timer in periodic
  // mode bit5,             interrupt 0=disable, 1=enable bit4=X, reserved
  // bits3-2=??,       input clock divider according to parameter
  // bit1,             0=16bit counter, 1=32-bit counter
  // bit0,             1=one shot mode, 0=wrapping mode
  TIMER32_CONTROL1 = BIT7 | BIT6 | BIT5 | div | BIT1;

  // interrupts enabled in the main program after all devices initialized
  // NVIC_IPR6
  NVIC_IPR6 = (NVIC_IPR6 & 0xFFFF00FF) | 0x00004000;  // priority 2

  // enable interrupt 25 in NVIC, NVIC_ISER0
  NVIC_ISER0 |= 1 << 25;

  end_critical(sr);
}

void T32_INT1_IRQHandler(void) {
  // acknowledge Timer32 Timer 1 interrupt
  TIMER32_INTCLR1 = 0;

  // execute user task
  (*Timer32_1_PeriodicTask)();

  // timer reload value to start the timer again
  TIMER32_LOAD1 = timer_1_rollover;
}
