/*
 * Based on "Real-time Chord Recognition for Live Performance" by
 * Adam Stark and Mark Plumbley.
 */

#ifndef CHROMAGRAM_H
#define CHROMAGRAM_H

#include "L138_LCDK_aic3106_init.h"
#include "L138_LCDK_switch_led.h"
#include "evmomapl138_gpio.h"

// Reference frequency corresponding to the lowest octave.
#define REFERENCE_FREQUENCY 130.81278265

// Size for FFT and chromagram calculations.
#define BUFFER_SIZE 2048

// Only consider 2 harmonics to improve accuracy across multiple instruments.
#define NUM_HARMONICS 2

// Consider two octaves, starting from the reference frequency.
#define NUM_OCTAVES 2

// Search the two bins on either side of a frequency for a maximum.
#define NUM_BINS_TO_SEARCH 2


// Collect 8192 audio samples per frame.
#define FRAME_SIZE 8192

// Sample at 16000 samples per second.
#define FS 16000


// Order for the low-pass filter.
#define ORDER 6

// Gain for the low-pass filter.
#define GAIN 4.802595226e+00


#define PI 3.14159265358979323


// Initialize the frequencies to look at and the chromagram vector.
void initialize();

// Downsample the input frame by a factor of four.
void downsample_frame(int16_t* input_audio_frame);

// Downsample and calculate the chromagram for the input audio frame.
void process_audio_frame(int16_t* input_audio_frame);

void make_hamming_window();

// Low-pass Butterworth filter applied to input audio.
void low_pass(int16_t* fft, int float_size);

/*
 * Calculate the square root of the magnitude of the FFT of the
 * audio signal.
 */
void calculate_magnitude_spectrum();

void calculate_chromagram();

// Return the chromagram as a float vector.
float* get_chromagram(int16_t* input_audio_frame);

// Returns 1 if the chromagram has been calculated, otherwise returns 0.
int is_ready();

extern float* downsampled_input_audio_frame;

extern float* window;
extern float* magnitude_spectrum;
extern float* chromagram;

// The 12 frequencies we consider.
extern float note_frequencies[12];

// Holds FFT values.
extern float y_real_sp[BUFFER_SIZE];
extern float y_imag_sp[BUFFER_SIZE];
extern float x_sp [2 * BUFFER_SIZE];

extern int downsampled_audio_frame_size;
extern int chroma_ready;

#endif
