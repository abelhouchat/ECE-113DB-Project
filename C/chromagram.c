#include "chromagram.h"

#include <ti/dsplib/dsplib.h>

#include <math.h>


float* downsampled_input_audio_frame;

float* window;
float* magnitude_spectrum;
float* chromagram;

float note_frequencies[12];

int downsampled_audio_frame_size;

int chroma_ready;


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

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// End of FFT stuff


void initialize()
{
	/*
	 * Initializes the chromagram vector and the frequencies to look
	 * at.
	 *
	 * Initializes the 12 frequencies to consider based on the reference
	 * frequency and allocates memory for the chromagram.
	 */

    int i;
    for (i = 0; i < 12; i++)
    {
    	note_frequencies[i] = REFERENCE_FREQUENCY * pow(2, (((float)i) / 12));
    }

    chromagram = (float*) malloc(12 * sizeof(float));

    chroma_ready = 0;
}

void downsample_frame(int16_t* input_audio_frame)
{
	/*
	 * Downsamples the input frame by a factor of four.
	 *
	 * Uses algorithm described by Adam Stark and Mark Plumbley.
	 */

    float* filtered_frame = (float*) malloc(FRAME_SIZE * sizeof(float));

    float b0 = 0.2929;
    float b1 = 0.5858;
    float b2 = 0.2929;
    float a2 = 0.1716;

    float x_1 = 0;
    float x_2 = 0;
    float y_1 = 0;
    float y_2 = 0;

    int i;
    for (i = 0; i < FRAME_SIZE; i++)
    {
    	filtered_frame[i] = input_audio_frame[i] * b0 + x_1 * b1 + x_2 * b2 - y_2 * a2;

        x_2 = x_1;
        x_1 = input_audio_frame[i];
        y_2 = y_1;
        y_1 = filtered_frame[i];
    }

    for (i = 0; i < FRAME_SIZE / 4; i++)
    {
        downsampled_input_audio_frame[i] = filtered_frame[i * 4];
    }

    free(filtered_frame);
}

void process_audio_frame(int16_t* input_audio_frame)
{
	/*
	 * Processes the audio input and generate the chromagram.
	 *
	 * First downsamples the input audio signal and calculates the
	 * magnitude of the FFT, then generates the chromagram.
	 */

    chroma_ready = 0;

    downsampled_audio_frame_size = FRAME_SIZE / 4;
	downsampled_input_audio_frame = (float*) malloc(downsampled_audio_frame_size * sizeof(float));
    downsample_frame(input_audio_frame);

    int i;
    for (i = 0; i < BUFFER_SIZE; i++)
    {
    	x_sp[2 * i] = downsampled_input_audio_frame[i];
    	x_sp[2 * i + 1] = 0;
    }

    free(downsampled_input_audio_frame);

    calculate_chromagram();
}

void calculate_magnitude_spectrum()
{
	/*
	 * Calculates the square root of the magnitude of the FFT of the
	 * audio signal.
	 */

    magnitude_spectrum = (float*) malloc((BUFFER_SIZE / 2 + 1) * sizeof(float));

    make_hamming_window();

    int i;
    for (i = 0; i < BUFFER_SIZE; i++)
    {
    	x_sp[2 * i] *= window[i];
    }

    free(window);

    perform_fft();

    int n;
    for (n = 0; n < (BUFFER_SIZE / 2) + 1; n++)
	{
    	magnitude_spectrum[n] = sqrt(sqrt(y_real_sp[n] * y_real_sp[n] + y_imag_sp[n] * y_imag_sp[n]));
	}
}

void calculate_chromagram()
{
	/*
	 * Calculates the chromagram based off the algorithm described
	 * by Adam Stark and Mark Plumbley.
	 *
	 * First we generate the magnitude spectrum of the FFT. Then,
	 * for each of the 12 frequencies generated by initialize(), we
	 * search over 2 octaves and for each octave we search over 2
	 * harmonics. For each harmonic, we look at the two bins on
	 * either side of the frequency and look for the maximum value
	 * in the magnitude spectrum. The chroma value for each frequency
	 * is generated by summing the maxima over harmonics and octaves.
	 */

    calculate_magnitude_spectrum();

    float divisor_ratio = (FS / 4.0) / ((float)BUFFER_SIZE);

    int n;
    for (n = 0; n < 12; n++)
    {
        float chroma_sum = 0.0;

        int octave;
        for (octave = 1; octave <= NUM_OCTAVES; octave++)
        {
            float note_sum = 0.0;

            int harmonic;
            for (harmonic = 1; harmonic <= NUM_HARMONICS; harmonic++)
            {
                int center_bin = round((note_frequencies[n] * octave * harmonic) / divisor_ratio);
                int min_bin = center_bin - (NUM_BINS_TO_SEARCH * harmonic);
                int max_bin = center_bin + (NUM_BINS_TO_SEARCH * harmonic);

                float max_val = 0.0;

                int k;
                for (k = min_bin; k < max_bin; k++)
                {
                    if (magnitude_spectrum[k] > max_val)
                    {
                    	max_val = magnitude_spectrum[k];
                    }
                }

                note_sum += (max_val / (float) harmonic);
            }

            chroma_sum += note_sum;
        }

        chromagram[n] = chroma_sum;
    }

    free(magnitude_spectrum);

    chroma_ready = 1;
}

float* get_chromagram(int16_t* input_audio_frame)
{
	/* Returns the chromagram as a float vector. */

	process_audio_frame(input_audio_frame);
    return chromagram;
}

int is_ready()
{
	/* Returns 1 if the chromagram has been calculated, otherwise returns 0. */

    return chroma_ready;
}

void make_hamming_window()
{
	/* Generates the Hamming window. */

    window = (float*) malloc(BUFFER_SIZE * sizeof(float));

    int n;
    for (n = 0; n < BUFFER_SIZE; n++)
    {
    	window[n] = 0.54 - 0.46 * cos(2 * PI * (((float)n) / ((float) BUFFER_SIZE)));
    }
}

void low_pass(int16_t* signal, int float_size)
{
	/*
	 * Low-pass Butterworth filter applied to input audio.
	 * Generated from www-users.cs.york.ac.uk/~fisher/mkfilter,
	 * which uses the bilinear transform to generate a filter.
	 *
	 * Cutoff frequency at 6000 Hz.
	 */

	float* xv = (float*)malloc((ORDER + 1) * sizeof(float));
	float* yv = (float*)malloc((ORDER + 1) * sizeof(float));

	int i;
	for (i = 0; i < float_size; i++)
	{
		xv[0] = xv[1];
		xv[1] = xv[2];
		xv[2] = xv[3];
		xv[3] = xv[4];
		xv[4] = xv[5];
		xv[5] = xv[6];
		xv[6] = signal[i] / GAIN;
		yv[0] = yv[1];
		yv[1] = yv[2];
		yv[2] = yv[3];
		yv[3] = yv[4];
		yv[4] = yv[5];
		yv[5] = yv[6];
		yv[6] = (xv[0] + xv[6])
				+ 6 * (xv[1] + xv[5])
				+ 15 * (xv[2] + xv[4])
				+ 20 * xv[3]
			    + ( -0.0433569884 * yv[0]) + ( -0.3911172306 * yv[1])
			    + ( -1.5172788447 * yv[2]) + ( -3.2597642798 * yv[3])
			    + ( -4.1360809983 * yv[4]) + ( -2.9785299261 * yv[5]);
		signal[i] = (int16_t)yv[6];
	}

	free(xv);
	free(yv);
}
