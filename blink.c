#include <msp430g2452.h>

void main(void) {
	// Stop the watchdog timer
	WDTCTL = WDTPW + WDTHOLD;

	// The output LED starts in the off state.
	P1DIR |= BIT6;

	// Interrupt on Pin 1.0 when it goes high.
	P1DIR &= ~BIT3;
	P1IES |= BIT3;
	P1REN |= BIT3;					// Enable internal pull-up/down resistors
  	P1OUT |= BIT3;					// Pull down the Pin internally
	P1IE |= BIT3;
	P1IFG &= ~BIT3;

	_BIS_SR(CPUOFF + GIE);		// Enter LPM0 w/ interrupt
	__no_operation();
	while (1) {
	}
}

void __attribute__((interrupt(PORT1_VECTOR))) p1_interrupt(void) {
	P1OUT ^= BIT6;

	P1IFG &= ~BIT3;
}
