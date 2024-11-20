#ifndef _CORTEX_M_H_
#define _CORTEX_M_H_

void disable_interrupts(void);
void enable_interrupts(void);
long start_critical(void);
void end_critical(long sr);
void wait_for_interrupt(void);

#endif
