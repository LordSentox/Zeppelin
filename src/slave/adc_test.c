/*******************************************************************************
 *
 * Test program to see how to work the ADC, checking its usages and whatnot.
 *
 ******************************************************************************/

#include <msp430g2553.h>
#include "../ssbl.h"

#ifndef NULL
#define NULL 0x0
#endif

#define input_range 32

void main(void) {
	// Stop the watchdog timer
	WDTCTL = WDTPW + WDTHOLD;

	// Set Up the test LED.
	P1DIR |= BIT4;

	// Begin receiving data
	ssbl_begin_read();

	for (;;) {
		// Always disable and the enable interrupts to make sure the package is
		// not changed while we are reading from it.
		// TODO: This might be unnecessary, since we're not going for absolute accuracy.
		_BIC_SR(GIE);

		// check the second value for an arbitrarily chosen value.
		if (ssbl_read_values[1] > input_range / 2) {
			P1OUT |= BIT4;
		}
		else {
			P1OUT &= ~BIT4;
		}

		_BIS_SR(GIE);
		__delay_cycles(50000);
	}
}
