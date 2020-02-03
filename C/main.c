#include <stdint.h>
#include <stdio.h>

#include "L138_LCDK_aic3106_init.h"
#include "L138_LCDK_switch_led.h"
#include "evmomapl138_gpio.h"

#include "chord_detection.h"
#include "chromagram.h"

float audio_frame[FRAME_SIZE];
uint16_t counter = 0;

interrupt void interrupt4(void) // interrupt service routine
{
    if (counter < FRAME_SIZE)
    {
        audio_frame[counter] = input_right_sample();
        counter++;
        output_right_sample(0);
    }
    else
        output_right_sample(0);

    return;
}

int main(void)
{
	L138_initialise_intr(FS_16000_HZ, ADC_GAIN_0DB, DAC_ATTEN_0DB, LCDK_LINE_INPUT);

    initialize();

	while(1)
	{
		if (counter == FRAME_SIZE)
		{
			get_chromagram(audio_frame);
			char* chord = find_chord(chromagram);
			printf("%s\n", chord);
			counter = 0;
			L138_initialise_intr(FS_16000_HZ, ADC_GAIN_0DB, DAC_ATTEN_0DB, LCDK_LINE_INPUT);
		}
	}
}
