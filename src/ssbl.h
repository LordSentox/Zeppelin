/*
 * Slow Single Bit Line
 *
 * This file implements this very simple and quite slow protocol optimised for
 * simple communications that don't require a great throughput.
 *
 * The operation is asynchronous and requires only a single data line.
 * Transfer is organised in Packages so that receiving may be started at any
 * point in time and beginning and end are still intelligable.
 *
 * The connection does Not guarantee or Try for every packet to be delivered.
 */

#ifndef SSBL_H
#define SSBL_H

// Obviously, the two chips must be running on a similar clockspeed for this
// to work, but if you had proper timers you would probably not use this protocol.
#define TRUE_CYCLES  10000
#define FALSE_CYCLES 5000

#define BIT_GAP      10000
#define PACKAGE_GAP  20000

// The possible derivation in Cycles information might have.
// This should not be ecceedingly high, since that might produce overlap errors.
#define AE 2500		// Acceptable Error

// Pin definitions. These might be better to be defined or given as an Argument
// by the software for later applications or in case I need multiple lines.
#define PIN_OUT P1OUT
#define BIT_OUT BIT0
#define PIN_IN  P1IN
#define BIT_IN  BIT0
#define ERR_BIT	BIT6

#define NUM_VALUES	3	// Number of values in a package
#define LEN_VALUE	5	// Number of bits in a value

// Write a package to the output.
// This function Blocks until the values have been written Completely.
void ssbl_write(char *values);

// Try to read a package from the Input.
// Blocks until One package has been read from the input.
// Returns 0 if everything went ok, or an Error-Code if an error occured while
// trying to read.
//int read_package(struct Package *p);

char ssbl_read_values[NUM_VALUES];
void ssbl_begin_read();
void ssbl_end_read();

#endif
