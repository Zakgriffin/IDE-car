#include "bluetooth.h"

#define BAUD_RATE 9600   

void init_bluetooth()
{
	//Set the UART to RESET state (set bit0 of EUSCI_A0->CTLW0 register to '1'
	EUSCI_A2->CTLW0 |= BIT0;

	// bit15=0,      no parity bits
	EUSCI_A2->CTLW0 &= ~BITF;
	// bit14=x,      not used when parity is disabled
	
	// bit13=0,      LSB first
	EUSCI_A2->CTLW0 &= ~BITD;
	// bit12=0,      8-bit data length
	EUSCI_A2->CTLW0 &= ~BITC;
	// bit11=0,      1 stop bit
	EUSCI_A2->CTLW0 &= ~BITB;
	// bits10-8=000, asynchronous UART mode
	EUSCI_A2->CTLW0 &= ~BITA;
	EUSCI_A2->CTLW0 &= ~BIT9;
	EUSCI_A2->CTLW0 &= ~BIT8;
	// bits7-6=11,   clock source to SMCLK
	EUSCI_A2->CTLW0 |= BIT7;
	EUSCI_A2->CTLW0 |= BIT6;
	// bit5=0,       reject erroneous characters and do not set flag
	EUSCI_A2->CTLW0 &= ~BIT5;
	// bit4=0,       do not set flag for break characters
	EUSCI_A2->CTLW0 &= ~BIT4;
	// bit3=0,       not dormant
	EUSCI_A2->CTLW0 &= ~BIT3;
	// bit2=0,       transmit data, not address (not used here)
	EUSCI_A2->CTLW0 &= ~BIT2;
	// bit1=0,       do not transmit break (not used here)
	EUSCI_A2->CTLW0 &= ~BIT1;
	// bit0=1,       hold logic in reset state while configuring
	// set CTLW0 - hold logic and configure clock source to SMCLK
	
	// baud rate
	// N = clock/baud rate = clock_speed/BAUD_RATE
	// set BRW register
	EUSCI_A2->BRW |= SystemCoreClock/BAUD_RATE;

	 // clear first and second modulation stage bit fields
	// MCTLW register;  
	EUSCI_A2->MCTLW &= 0xF;
	

	// P1.3 = TxD
	// P1.2 = RxD
	// we will be using P1.2, P1.3 for RX and TX but not in I/O mode, so we set Port1 SEL1=0 and SEL0=1
	// set SEL0, SEL1 appropriately
	P3->SEL1 &= ~BIT2;
	P3->SEL0 |= BIT2;
	
	P3->SEL1 &= ~BIT3;
	P3->SEL0 |= BIT3;
	

	// CTLW0 register - release from reset state
	EUSCI_A2->CTLW0 &= ~BIT0;

	// disable interrupts (transmit ready, start received, transmit empty, receive full)	
	// IE register;      
	EUSCI_A2->IE &= ~BIT3;
	EUSCI_A2->IE &= ~BIT2;
	EUSCI_A2->IE &= ~BIT1;
	EUSCI_A2->IE &= ~BIT0;

}

Byte getchar_bluetooth()
{
	Byte inChar;
	// Wait for data
	// IFG register
	while((EUSCI_A2->IFG & BIT0) == 0)
	{
		//witing
	}
	
	// read character and store in inChar variable
	// RXBUF register
	inChar = EUSCI_A2->RXBUF;
	
	//Return the 8-bit data from the receiver 
	return(inChar);

}

void putchar_bluetooth(char ch)
{
	// Wait until transmission of previous bit is complete 
	// IFG register
	while((EUSCI_A2->IFG & BIT1) == 0)
	{
		//witing
	}
	
	// send ch character to uart
	// TXBUF register 
	EUSCI_A2->TXBUF = ch;
}

void put_bluetooth(char *ptr_str)
{
	while(*ptr_str != 0)
		putchar_bluetooth(*ptr_str++);
}

bool dataAvailable_bluetooth(){
	return ((EUSCI_A2->IFG & BIT0) != 0);
}
