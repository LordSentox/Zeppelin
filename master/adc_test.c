/*******************************************************************************
 *
 * Test program to see how to work the ADC, checking its usages and whatnot.
 *
 ******************************************************************************/

#include <msp430g2553.h>
#include "input.h"

#define MAX_AC_VAL 1024.0

void update_display(float val) {
	// Switch the LEDs on according to the level of voltage on the Input.
	if (val >= 0.75) P1OUT = BIT6;
	else if (val >= 0.5) P1OUT = (BIT0 + BIT6);
	else if (val >= 0.25) P1OUT = BIT0;
	else P1OUT = 0x0;
}

void main(void) {
	// Stop the watchdog timer
	WDTCTL = WDTPW + WDTHOLD;

	// Set Up the test LEDs.
	P1DIR |= (BIT0 + BIT6);
	P1OUT &= ~(BIT0 + BIT6);

	init_adc_input();

	// Get the latest Conversion ASAP.
	for (;;) {
		// Start a new conversion and wait for it to finish.
		ADC10CTL0 |= ADC10SC;
		while ((ADC10CTL1 & ADC10BUSY) == 0x01);

		// update the display.
		update_display((float)ADC10MEM/MAX_AC_VAL);
	}
}
