#ifndef INPUT_H
#define INPUT_H

#define MAX_ADC_VAL 1024

// The axis are as follows:
// Z describes the Positions from the Back to the Front of the Zeppelin. (Positive is front)
// H describes the total height of the Zeppelin, usually from Ground.
// Y describes the current yaw of the Zeppelin, to an arbitrary starting reference. (Positive is right viewed from top)
volatile char delta_Z;
volatile char delta_H;
volatile char delta_Y;

// This function is called once, when you want to start reading the input from
// your Input Potentiometers.
void adc_init_inputs();

// Read the current values of the analog inputs. The values are between 0 and 32.
// 0 is the Maximum speed in the opposite direction
void adc_update_values();

#endif
