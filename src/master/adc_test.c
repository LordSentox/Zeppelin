/*******************************************************************************
 *
 * Test program to see how to work the ADC, checking its usages and whatnot.
 *
 ******************************************************************************/

#include <msp430g2553.h>
#include "input.h"
#include "../ssbl.h"

#define input_range 32

void main(void) {
	// Stop the watchdog timer
	WDTCTL = WDTPW + WDTHOLD;

	// Set Up the test LED.
	P1DIR |= (BIT4 + BIT6);
	P1OUT &= ~(BIT4 + BIT6);

	adc_init_inputs();

	// Get the latest Conversion ASAP.
	for (;;) {
		adc_update_values();

		// Write the new values to the output.
		char values[3] = {delta_Z, delta_H, delta_Y};
		ssbl_write(values);

		// Test-LED, to see, how much in sync master and slave are.
		// (In terms of translation, the LED is on when the Zeppelin is going up)
		if (delta_H > input_range / 2) {
			P1OUT |= BIT4;
		}
		else {
			P1OUT &= ~BIT4;
		}

	}
}
