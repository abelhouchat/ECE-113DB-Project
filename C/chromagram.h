#ifndef CHROMAGRAM_H
#define CHROMAGRAM_H

#define REFERENCE_FREQUENCY 130.81278265
#define BUFFER_SIZE 2048
#define NUM_HARMONICS 2
#define NUM_OCTAVES 2
#define NUM_BINS_TO_SEARCH 2

#define FRAME_SIZE 8192
#define FS 16000

#define PI 3.14159265358979323


void initialize();

void downsample_frame(float* input_audio_frame);
void process_audio_frame(float* input_audio_frame);
void make_hamming_window();
void calculate_magnitude_spectrum();
void calculate_chromagram();

float* get_chromagram(float* input_audio_frame);

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
