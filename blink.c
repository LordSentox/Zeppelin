//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.2|-->LED
//
//  J. Stevenson
//  Texas Instruments, Inc
//  July 2011
//  Built with Code Composer Studio v5
//***************************************************************************************

#include <msp430g2553.h>

unsigned int timerCount = 0;
int main(void) {
	/* Initialisation */
	WDTCTL = WDTPW + WDTHOLD; // Stop the watchdog timer.
	P1DIR |= BIT0; // Set the red LED to an Output pin.
	P1OUT &= ~BIT0; // Turn the LED off initially.

	CCTL0 = CCIE; // Enable Interrupt for CCTL0.
	TACTL = TASSEL_2 | MC_2; // Set the Timer Input Selection to SMCLK and the mode to count upwards continuously.

	__enable_interrupt();
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer_a(void) {
	timerCount = (timerCount + 1) % 8;

	if (timerCount == 0) P1OUT ^= BIT0;
}
