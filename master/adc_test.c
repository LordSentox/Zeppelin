/*******************************************************************************
 *
 * Test program to see how to work the ADC, checking its usages and whatnot.
 *
 ******************************************************************************/

#include <msp430g2553.h>
#include "input.h"

#define input_range 32

void update_display() {
	// Switch the LEDs on according to the level of voltage on the Input.
	if (delta_Z >= 8 && delta_Z < 16) P1OUT = BIT6;
	else if (delta_Z >= 0) P1OUT = (BIT0 + BIT6);
	else if (delta_Z >= -8) P1OUT = BIT0;
	else P1OUT = 0;
}

void main(void) {
	// Stop the watchdog timer
	WDTCTL = WDTPW + WDTHOLD;

	// Set Up the test LEDs.
	P1DIR |= (BIT0 + BIT6);
	P1OUT &= ~(BIT0 + BIT6);

	adc_init_inputs();

	// Get the latest Conversion ASAP.
	for (;;) {
		adc_update_values();

		// update the display.
		update_display();
	}
}
