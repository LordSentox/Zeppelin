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
	P1DIR |= (BIT6);
	P1OUT &= ~(BIT6);

	adc_init_inputs();

	// Get the latest Conversion ASAP.
	for (;;) {
		adc_update_values();

		// Write the new values to the output.
		char values[3] = {delta_Z, delta_H, delta_Y};
		ssbl_write(values);
	}
}
