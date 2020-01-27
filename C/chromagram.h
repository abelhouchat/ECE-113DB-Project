#ifndef CHROMAGRAM_H
#define CHROMAGRAM_H

#define REFERENCE_FREQUENCY 130.81278265
#define BUFFER_SIZE 8192
#define NUM_HARMONICS 2
#define NUM_OCTAVES 2
#define NUM_BINS_TO_SEARCH 2

#define FRAME_SIZE 512
#define FS 16000

#define PI 3.14159265358979323

#include <math.h>
#include <ti/dsplib/dsplib.h>

void initialize(int16_t frame_size, int16_t fs);

void process_audio_frame(float* input_audio_frame);
void set_input_audio_frame_size(int16_t frame_size);
void set_sampling_frequency(int16_t fs);
void set_chroma_calculation_interval(int16_t num_samples);
float* get_chromagram();
int16_t is_ready();

void setup_fft();
void calculate_chromagram();
void calculate_magnitude_spectrum();
void downsample_frame(float* input_audio_frame);
void make_hamming_window();
float round(float val);

float* window;
float* magnitude_spectrum;
float* downsampled_input_audio_frame;
float* chromagram;

float note_frequencies[12];

int16_t downsampled_audio_frame_size;
int16_t num_samples_since_last_calculation;
int16_t chroma_calculation_interval;
int16_t chroma_ready;


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// FFT stuff


#pragma DATA_ALIGN(x_sp, 8);
float   x_sp [2 * BUFFER_SIZE];
#pragma DATA_ALIGN(y_sp, 8);
float   y_sp [2 * BUFFER_SIZE];
#pragma DATA_ALIGN(w_sp, 8);
float   w_sp [2 * BUFFER_SIZE];

// brev routine called by FFT routine
unsigned char brev[64] = {
    0x0, 0x20, 0x10, 0x30, 0x8, 0x28, 0x18, 0x38,
    0x4, 0x24, 0x14, 0x34, 0xc, 0x2c, 0x1c, 0x3c,
    0x2, 0x22, 0x12, 0x32, 0xa, 0x2a, 0x1a, 0x3a,
    0x6, 0x26, 0x16, 0x36, 0xe, 0x2e, 0x1e, 0x3e,
    0x1, 0x21, 0x11, 0x31, 0x9, 0x29, 0x19, 0x39,
    0x5, 0x25, 0x15, 0x35, 0xd, 0x2d, 0x1d, 0x3d,
    0x3, 0x23, 0x13, 0x33, 0xb, 0x2b, 0x1b, 0x3b,
    0x7, 0x27, 0x17, 0x37, 0xf, 0x2f, 0x1f, 0x3f
};

// The seperateRealImg function separates the real and imaginary data
// of the FFT output. This is needed so that the data can be plotted
// using the CCS graph feature
float y_real_sp[BUFFER_SIZE];
float y_imag_sp[BUFFER_SIZE];
float y_mag[BUFFER_SIZE];

separateRealImg()
{
    int i, j;

    for (i = 0, j = 0; j < BUFFER_SIZE; i += 2, j++)
    {
        y_real_sp[j] = y_sp[i];
        y_imag_sp[j] = y_sp[i + 1];
    }
}

// Function for generating sequence of twiddle factors
void gen_twiddle_fft_sp(float* w, int n)
{
    int i, j, k;
    double x_t, y_t, theta1, theta2, theta3;

    for (j = 1, k = 0; j <= n >> 2; j = j << 2)
    {
        for (i = 0; i < n >> 2; i += j)
        {
            theta1 = 2 * PI * i / n;
            x_t = cos(theta1);
            y_t = sin(theta1);
            w[k] = (float)x_t;
            w[k + 1] = (float)y_t;

            theta2 = 4 * PI * i / n;
            x_t = cos(theta2);
            y_t = sin(theta2);
            w[k + 2] = (float)x_t;
            w[k + 3] = (float)y_t;

            theta3 = 6 * PI * i / n;
            x_t = cos(theta3);
            y_t = sin(theta3);
            w[k + 4] = (float)x_t;
            w[k + 5] = (float)y_t;
            k += 6;
        }
    }
}

void perform_fft()
{
    gen_twiddle_fft_sp(w_sp, BUFFER_SIZE);
    DSPF_sp_fftSPxSP(BUFFER_SIZE, x_sp, w_sp, y_sp, brev, 4, 0, BUFFER_SIZE);
    separateRealImg();
}

#endif