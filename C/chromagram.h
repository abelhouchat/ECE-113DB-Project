#ifndef CHROMAGRAM_H
#define CHROMAGRAM_H

#include "L138_LCDK_aic3106_init.h"
#include "L138_LCDK_switch_led.h"
#include "evmomapl138_gpio.h"

#define REFERENCE_FREQUENCY 130.81278265
#define BUFFER_SIZE 2048
#define NUM_HARMONICS 2
#define NUM_OCTAVES 2
#define NUM_BINS_TO_SEARCH 2

#define FRAME_SIZE 8192
#define FS 16000
#define TIME_FRAME ((FRAME_SIZE + BUFFER_SIZE) / FS)

#define PI 3.14159265358979323


void initialize();

void downsample_frame(int16_t* input_audio_frame);
void process_audio_frame(int16_t* input_audio_frame);
void make_hamming_window();
void low_pass(int16_t* fft, int float_size, int order);
void calculate_magnitude_spectrum();
void calculate_chromagram();

float* get_chromagram(int16_t* input_audio_frame);

int is_ready();


extern float* downsampled_input_audio_frame;

extern float* window;
extern float* magnitude_spectrum;
extern float* chromagram;

extern float note_frequencies[12];

extern float y_real_sp[BUFFER_SIZE];
extern float y_imag_sp[BUFFER_SIZE];
extern float x_sp [2 * BUFFER_SIZE];

extern int downsampled_audio_frame_size;
extern int chroma_ready;

#endif
