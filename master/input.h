#ifndef INPUT_H
#define INPUT_H

void init_adc_input();

// Read the current values of the analog inputs. The values are between 0 and 32.
// 0 is the Maximum speed in the opposite direction
void read_values(unsigned short *forward, unsigned short *gain, unsigned short *yaw);

#endif
