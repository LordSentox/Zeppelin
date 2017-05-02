#include "input.h"

#include <msp430g2553.h>

#ifndef __MSP430_HAS_ADC10__
#error "The MSP430 version you are using must have ADC-10 registered."
#endif

void adc_init_inputs() {
	P1DIR &= ~(BIT1 + BIT2 + BIT3);	// Register P1.1 - P1.3 as inputs

	// ADC uses Voltage from GND to VCC. The Potentiometers should be connected
	// to make up a Voltage Divider between those Values.
	ADC10CTL0 = SREF_0 + ADC10SHT_2 + ADC10ON + ADC10IE;
	ADC10AE0 = BIT2 + BIT3 + BIT4;
}

int adc_do_conversion() {
	ADC10CTL0 |= ENC + ADC10SC;					// Enable and start a conversion.
	while ((ADC10CTL1 & ADC10BUSY) == 0x01);	// Wait until the ADC is finished.
	return ADC10MEM;
}

// Z-Axis control input is on P1.1
// Height is read from P1.3
// Yaw is read from input P1.4
void adc_update_values() {
	/* Read the Z-Axis movement control input. */
	ADC10CTL1 = INCH_1;							// Input Pin is Pin 1.1
	delta_Z = adc_do_conversion() / 32 - 15;	// Do the conversion and save.

	/* Read the Height movement control input. */
	ADC10CTL1 = INCH_2;							// Input Pin is Pin 1.2
	delta_H = adc_do_conversion() / 32 - 15;	// Do the conversion and save.

	/* Read the Yaw movement control input. */
	ADC10CTL1 = INCH_3;							// Input Pin is Pin 1.3
	delta_Y = adc_do_conversion() / 32 - 15;	// Do the conversion and save.

	// Disable ADC conversion input as long as it's not needed.
	ADC10CTL0 &= ~ENC;
}
