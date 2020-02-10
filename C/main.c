#include <stdint.h>
#include <stdio.h>

#include "L138_LCDK_aic3106_init.h"
#include "L138_LCDK_switch_led.h"
#include "evmomapl138_gpio.h"

#include "chord_detection.h"
#include "chromagram.h"
//#include "output.h"

int16_t audio_frame[FRAME_SIZE];
int16_t metronome[FRAME_SIZE];

char* chord;
//char* chord_sequence[120];

uint16_t counter = 0;
//uint16_t timer = 0;
uint16_t off = 0;

FILE* sheet;

interrupt void interrupt4(void) // interrupt service routine
{
    if (counter < FRAME_SIZE)
    {
        audio_frame[counter] = input_left_sample();
        counter++;
//        output_right_sample(0);
        output_right_sample(metronome[counter]);
    }
//    else if (counter < BUFFER_SIZE + FRAME_SIZE)
//    {
//    	audio_frame[counter - BUFFER_SIZE] = input_right_sample();
//    	counter++;
//    	output_right_sample(0);
//    }
    else
        output_left_sample(0);

    return;
}

int main(void)
{
	L138_initialise_intr(FS_16000_HZ, ADC_GAIN_6DB, DAC_ATTEN_0DB, LCDK_LINE_INPUT);

    initialize();

    int i, j;
    for (j = 0; j < 1000; j++)
    {
		for (i = j; i < j + 50; i++)
		{
			metronome[i] = 10000;
		}
		for (i = j + 50; i < j + 100; i++)
		{
			metronome[i] = -10000;
		}
		j += 100;
    }
    for (i = 1000; i < FRAME_SIZE; i++)
    {
    	metronome[i] = 0;
    }

	while(1)
	{
		if (counter == FRAME_SIZE)
		{
			low_pass(audio_frame, FRAME_SIZE, 6);
			get_chromagram(audio_frame);
//			int i;
//			for (i = 0; i < 12; i++)
//			{
//				printf("%f\n", chromagram[i]);
//			}
			chord = find_chord(chromagram);
			printf("%s\n", chord);
//			chord_sequence[timer] = chord;
//			timer++;
			counter = 0;
			L138_initialise_intr(FS_16000_HZ, ADC_GAIN_6DB, DAC_ATTEN_0DB, LCDK_LINE_INPUT);
		}
	}
//
//	sheet = fopen("chord_sequence.txt", "w");
//	int i;
//	for (i = 0; i < 120; i++)
//	{
//		fprintf(sheet, "%s\n", chord_sequence[i]);
//	}
//	fclose(sheet);
}
