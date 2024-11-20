#include "cortex_m.h"

/*
__asm void __weak disable_interrupts(void) { CPSID I bx lr }

__asm void __weak enable_interrupts(void) { CPSIE I bx lr }

// code between start_critical and end_critical is run atomically

// copies/returns PRIMASK (I bit), disables interrupts
__asm long __weak start_critical(void) { MRS R0, PRIMASK CPSID I bx lr }

// end_critical sets PRIMASK with value passed in
__asm void __weak end_critical(long sr) { MSR PRIMASK, R0 bx lr }

// enters low power sleep mode waiting for interrupt (WFI instruction)
// returns after ISR has been run
__asm void __weak wait_for_interrupt(void) { WFI bx lr }
*/

__asm void
__weak disable_interrupts(void)
{
	CPSID   I
	bx      lr
}

__asm void
__weak enable_interrupts(void)
{
	CPSIE   I
	bx      lr
}

__asm long
__weak start_critical(void)
{
	MRS     R0, PRIMASK
	CPSID   I
	bx      lr
}

__asm void
__weak end_critical(long sr)
{
	MSR     PRIMASK, R0
	bx      lr
}

__asm void
__weak wait_for_interrupt(void)
{
	WFI
	bx      lr
}

// ZZZ come back reformat
