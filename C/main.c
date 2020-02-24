#include <stdint.h>
#include <stdio.h>

#include "L138_LCDK_aic3106_init.h"
#include "L138_LCDK_switch_led.h"
#include "evmomapl138_gpio.h"

#include "chord_detection.h"
#include "chromagram.h"

// Number of initial samples to disregard.
#define THROWAWAY 1500

// How many loops to run the program for.
#define RUNTIME 75

int16_t audio_frame[FRAME_SIZE];

// Store a square wave to act as a metronome.
int16_t metronome[FRAME_SIZE + THROWAWAY];

char* chord;

// Store the sequence of chords over the program's runtime.
char* chord_sequence[RUNTIME];

// Keeps track of when the frame is filled.
uint16_t counter = 0;

// Keeps track of how many loops the program has run for.
uint16_t iterations = 0;

// 0 if the program should continue, 1 if it should terminate.
uint16_t off = 0;

// File to write chord sequence to.
FILE* sheet;

interrupt void interrupt4(void) // interrupt service routine
{
	/*
	 * Because the first ~1500 samples are garbage, we store them
	 * then overwrite them. Then we store the subsequent frame of
	 * audio.
	 *
	 * We also continuously play the metronome so that the user
	 * will know when to play the chord.
	 */
	if (counter < THROWAWAY)
	{
		audio_frame[counter] = input_left_sample();
		counter++;
		output_right_sample(metronome[counter]);
	}
	else if (counter < FRAME_SIZE + THROWAWAY)
    {
        audio_frame[counter - THROWAWAY] = input_left_sample();
        counter++;
        output_right_sample(metronome[counter]);
    }
    else
        output_left_sample(0);

    return;
}

int main(void)
{
	L138_initialise_intr(FS_16000_HZ, ADC_GAIN_6DB, DAC_ATTEN_0DB, LCDK_MIC_INPUT);

    initialize();

    // Initialize the audio for the metronome.
    int i, j;
    for (i = 0; i < THROWAWAY; i++)
    {
    	metronome[i] = 0;
    }
    for (j = THROWAWAY; j < THROWAWAY + 1000; j++)
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
    for (i = THROWAWAY + 1000; i < FRAME_SIZE + THROWAWAY; i++)
    {
    	metronome[i] = 0;
    }

	while(1)
    {
		/*
		 * Once a frame of audio has been filled, low-pass it, find
		 * the chromagram, and find the chord. Print the chord to the
		 * console and store the chord in the chord_sequence array.
		 * console Then reset the counter and increment the number of
		 * console iterations.
		 */
		if (counter == FRAME_SIZE + THROWAWAY && iterations < RUNTIME)
		{
			low_pass(audio_frame, FRAME_SIZE);
			get_chromagram(audio_frame);
			chord = find_chord(chromagram);
			printf("%s\n", chord);

		    chord_sequence[iterations] = chord;

			counter = 0;
			L138_initialise_intr(FS_16000_HZ, ADC_GAIN_6DB, DAC_ATTEN_0DB, LCDK_MIC_INPUT);
			iterations++;
		}
		/*
		 * Once the maximum number of iterations has been reached,
		 * write the sequence of chords with time stamps to a text
		 * file.
		 */
		else if (iterations == RUNTIME && !off)
		{
			// Approximate the time (in seconds) between iterations.
			float time_diff = (FRAME_SIZE + THROWAWAY) / (float)FS;

			char* current_chord = chord_sequence[0];

			sheet = fopen("chord_sequence.txt", "w");

			// Calculates the time stamps.
			uint16_t timer = 0;

			/*
			 * If the next chord is the same as the previous chord,
			 * do nothing but keep track of how long it has been
			 * playing for. Once the chord changes, write down the
			 * chord and how long it played for, given by the
			 * difference between i and timer. Then update the
			 * current chord and update timer.
			 *
			 * Example:
			 * 	NC: 0.00 - 0.60
			 * 	C: 0.60 - 3.40
			 * 	Am#: 3.40 - 4.60
			 */
			for (i = 1; i < iterations; i++)
			{
				if (chord_sequence[i] != current_chord)
				{
					fprintf(sheet, "%s: %.2f - %.2f\n", current_chord, timer * time_diff, i * time_diff);
					current_chord = chord_sequence[i];
					timer = i;
				}
			}
			// Print the final chord.
			fprintf(sheet, "%s: %.2f - %.2f\n", chord_sequence[iterations - 1], timer * time_diff, iterations * time_diff);
			fclose(sheet);

			off = 1;

			// Write to console to let the user know the program is done.
			printf("Done :)\n");
		}
		// Once the file is written, do nothing more.
		else
		{

		}
	}
}
