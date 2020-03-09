/*
 * Based on "Real-time Chord Recognition for Live Performance" by
 * Adam Stark and Mark Plumbley.
 */

#ifndef CHROMAGRAM_H
#define CHROMAGRAM_H

#include "L138_LCDK_aic3106_init.h"
#include "L138_LCDK_switch_led.h"
#include "evmomapl138_gpio.h"


#define FRAME_SIZE 4096
	/* Number of samples to store at once. */
#define FS 16000
	/* Sampling rate. */

#define REFERENCE_FREQUENCY 130.8127826
	/* Reference frequency corresponding to the lowest octave. */
#define BUFFER_SIZE 1024
	/*
	 * Size for FFT and chromagram calculations. Must be one-fourth
	 * of FRAME_SIZE.
	 */
#define NUM_HARMONICS 2
	/* Number of harmonics we search over. */
#define NUM_OCTAVES 2
	/*
	 * Number of octaves we search over, starting with the
	 * reference frequency.
	 */
#define NUM_BINS_TO_SEARCH 2
	/*
	 * Number of bins to search on either side of a frequency for
	 * a maximum.
	 */

#define ORDER 6
	/* Order for the low-pass filter. */
#define GAIN 4.802595226e+00
	/* Gain for the low-pass filter. */

#define PI 3.14159265358979323


void initialize();
	/*
	 * Initializes the chromagram vector and the frequencies to look
	 * at.
	 */

void downsample_frame(int16_t* input_audio_frame);
	/* Downsamples the input frame by a factor of four. */

void process_audio_frame(int16_t* input_audio_frame);
	/*
	 * Downsamples and calculates the chromagram for the input audio
	 * frame.
	 */

void calculate_magnitude_spectrum();
	/*
	 * Calculates the square root of the magnitude of the FFT of the
	 * audio signal.
	 */

void calculate_chromagram();
	/* Calculates the chromagram. */

float* get_chromagram(int16_t* input_audio_frame);
	/* Returns the chromagram as a float vector. */

int is_ready();
	/*
	 * Returns 1 if the chromagram has been calculated, otherwise
	 * returns 0.
	 */

void make_hamming_window();
	/* Generates the Hamming window. */

void low_pass(int16_t* fft, int float_size);
	/* Low-pass Butterworth filter applied to input audio. */


extern float* downsampled_input_audio_frame;

extern float* window;
extern float* magnitude_spectrum;
extern float* chromagram;

extern float note_frequencies[12];

extern float y_real_sp[BUFFER_SIZE];
extern float y_imag_sp[BUFFER_SIZE];
extern float x_sp[2 * BUFFER_SIZE];

extern int downsampled_audio_frame_size;

extern int chroma_ready;

#endif
