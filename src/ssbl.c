#include "ssbl.h"
#include <msp430g2553.h>

// Check that the MSP-Version has the required Ports.
#ifndef __MSP430_HAS_PORT1_R__
#error "Your msp430 must have a Port 1 for the connection to work"
#endif

#ifndef NULL
#define NULL 0x0
#endif

/* Package write */

// TODO: Recreate this functionality with CC-Interrupts
void write_val(int value) {
	for (unsigned short i = 0; i < LEN_VALUE; ++i) {
		if ((value >> i) & 1) {
			PIN_OUT |= BIT_OUT;
			__delay_cycles(TRUE_CYCLES);
		}
		else {
			PIN_OUT |= BIT_OUT;
			__delay_cycles(FALSE_CYCLES);
		}
		PIN_OUT &= ~BIT_OUT;

		__delay_cycles(BIT_GAP);
	}
}

void ssbl_write(char *values) {
	// TODO: This should be in a setup function.
	P1OUT &= ~BIT_OUT;
	P1DIR |= BIT_OUT;

	for (unsigned int i = 0; i < NUM_VALUES; ++i) {
		write_val(values[i]);
	}

	__delay_cycles(PACKAGE_GAP);
}

/* Package read */

unsigned int num_current_bit;		// The position of the bit currently to be read.
char current_values[NUM_VALUES];	// The values that are in the process of reading.

// Reset the current package that is being read.
void reset_current_package() {
	num_current_bit = 0;
	for (int i = 0; i < NUM_VALUES; ++i) {
		current_values[i] = 0;
	}
}

void __attribute__((interrupt(PORT1_VECTOR))) p1_interrupt(void) {
	unsigned long delta_time = TAR;		// Save the current timer value (Input divider is 8)
	TAR = 0;							// Reset the time counter

	// Check which edge has caused the interrupt.
	if ((P1IES & BIT_IN) == 0x0) {
		// The edge is lo/hi
		// First, check if this is the start of a new package. In this case, we
		// only check for a greater than time, because we don't really care about
		// the packet rate of the sender.
		if (delta_time > PACKAGE_GAP - AE) {
			// Check if the current package is valid and replace the old package
			// with it if appropriate.
			if (num_current_bit == NUM_VALUES * LEN_VALUE) {
				// The package should have been read correctly, so we will
				// update the latest values from our data.
				for (int i = 0; i < NUM_VALUES; ++i) {
					// TODO: This is probably not the most efficient way!
					ssbl_read_values[i] = current_values[i];
				}

				reset_current_package();
			}
			else {
				// An error has occured while reading the package.
				P1OUT ^= ERR_BIT;	// Flip the error bit

				reset_current_package();
			}
		}
		// The only other possibility is that it is the beginning of a new bit.
		// If that is not the case, this is an error.
		else if (delta_time <= BIT_GAP - AE || delta_time >= BIT_GAP + AE) {
			P1OUT ^= ERR_BIT;		// Flip the error bit
			reset_current_package();
		}
	}
	else {
		// The edge is hi/lo
		if (delta_time > TRUE_CYCLES - AE && delta_time < TRUE_CYCLES + AE) {
			current_values[num_current_bit / LEN_VALUE] |= 1 << num_current_bit % LEN_VALUE;
		}
		else if (delta_time > FALSE_CYCLES - AE && delta_time < FALSE_CYCLES + AE) {
			// TODO: This is why we don't always have to put in all zeroes at
			// the start of a new package. Alternatively this could possibly be removed.
			current_values[num_current_bit / LEN_VALUE] &= ~(1 << num_current_bit % LEN_VALUE);
		}
		else {
			P1OUT ^= ERR_BIT;		// Flip the error bit
			reset_current_package();
		}

		++num_current_bit;
	}

	P1IES ^= BIT_IN;	// Switch the edge-mode to detect the next change.
	P1IFG &= ~BIT_IN;	// Reset the interrupt flag
}

void ssbl_begin_read() {
	// Prepare the error output bit, so the user has a way of knowing wtfigw.
	P1DIR |= ERR_BIT;
	P1OUT &= ~ERR_BIT;

	// Initialise a first package to begin reading
	reset_current_package();

	// Start the timer
	TACTL = MC_2 | ID_0 | TASSEL_2;

	// Prepare the Port for the read process
	P1DIR &= ~BIT_IN;	// Set Pin to Input direction
	P1REN |= BIT_IN;	// Activate internal pulling resistor

	// Initial interrupt is going to be hi/lo, which is the best to immediately
	// detect the first package.
	P1IES |= BIT_IN;	// Select the hi/lo edge for the interrupt
	P1OUT |= BIT_IN;	// Select pull-up mode
	P1IFG &= ~BIT_IN;	// Set Interrupt flag to begin listening.
	P1IE  |= BIT_IN;	// Enable interrupts on the selected pin.

	// The entire reading process will be handled by interrupts
	_BIS_SR(GIE);
}

void ssbl_end_read() {
	P1REN  &= ~BIT_IN;
	P1IE &= ~BIT_IN;
}
