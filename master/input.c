#include "input.h"

#include <msp430g2553.h>

void init_inputs() {
	// Initialise the ADC.
	ADC10CTL0 = SREF_0 + ADC10SHT_2 + ADC10ON;

	ADC10CTL1 = INCH_1 + SHS_0 + ADC10SSEL_0 + ADC10DIV_0 + CONSEQ_0;
	ADC10AE0 = BIT1;

	// Enable Conversions.
	ADC10CTL0 |= ENC;
}

void read_values(unsigned short *forward, unsigned short *gain, unsigned short *yaw) {

}
