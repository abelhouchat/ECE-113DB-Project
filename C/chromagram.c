#include "chromagram.h"

void initialize()
{
    for (int16_t i = 0; i < 12; i++)
        note_frequencies[i] = REFERENCE_FREQUENCY * pow(2, (((float)i) / 12));
    
    // setup_fft();

    // buffer = (float*) malloc(BUFFER_SIZE * sizeof(float));

    chromagram = (float*) malloc(BUFFER_SIZE * sizeof(float));

    // for (int16_t i = 0; i < 12; i++)
    //     chromagram[i] = 0.0;

    // magnitude_spectrum = (float*) malloc((BUFFER_SIZE / 2) + 1);

    // make_hamming_window();

    num_samples_since_last_calculation = 0;

    chroma_calculation_interval = 4096;

    chroma_ready = 0;
}

void set_input_audio_frame_size()
{
    downsampled_audio_frame_size = FRAME_SIZE / 4;
    downsampled_input_audio_frame = (float*) malloc(downsampled_audio_frame_size * sizeof(float));
}

double* get_chromagram()
{
    return chromagram;
}

int16_t is_ready()
{
    return chroma_ready;
}

void process_audio_frame(float* input_audio_frame)
{
    chroma_ready = 0;
    downsample_frame(input_audio_frame);
    for (int16_t i = 0; i < BUFFER_SIZE - downsampled_audio_frame_size; i++)
    {
        x_sp[2 * i] = x_sp[2 * (i + downsampled_audio_frame_size)];
        x_sp[2 * i + 1] = x_sp[2 * (i + downsampled_audio_frame_size) + 1];
    }
    
    int16_t n = 0;
    for (int16_t i = (BUFFER_SIZE - downsampled_audio_frame_size); i < BUFFER_SIZE; i++)
    {
        x_sp[2 * i] = downsampled_input_audio_frame[n];
        x_sp[2 * i + 1] = 0;
        n++;
    }

    free(downsampled_input_audio_frame);

    num_samples_since_last_calculation += FRAME_SIZE;

    if (num_samples_since_last_calculation >= chroma_calculation_interval)
    {
        calculate_chromagram();
        num_samples_since_last_calculation = 0;
    }
}

void downsample_frame(float* input_audio_frame)
{
    float* filtered_frame = (float*) malloc(FRAME_SIZE * sizeof(float));

    float b0 = 0.2929;
    float b1 = 0.5858;
    float b2 = 0.2929;
    float a1 = -0.0000;
    float a2 = 0.1716;
    
    float x_1 = 0;
    float x_2 = 0;
    float y_1 = 0;
    float y_2 = 0;

    for (int16_t i = 0; i < FRAME_SIZE; i++)
    {
        filtered_frame[i] = input_audio_frame[i] * b0 + x_1 * b1 + x_2 * b2 - y_1 * a1 - y_2 * a2;
        
        x_2 = x_1;
        x_1 = input_audio_frame[i];
        y_2 = y_1;
        y_1 = filtered_frame[i];
    }
    
    for (int16_t i = 0; i < FRAME_SIZE / 4; i++)
    {
        downsampled_input_audio_frame[i] = filtered_frame[i * 4];
    }

    free(filtered_frame);
}

void calculate_chromagram()
{
    calculate_magnitude_spectrum();

    float divisor_ratio = (FS / 4.0) / ((float)BUFFER_SIZE);

    for (int16_t n = 0; n < 12; n++)
    {
        float chroma_sum = 0.0;

        for (int16_t octave = 1; octave <= NUM_OCTAVES; octave++)
        {
            float note_sum = 0.0;

            for (int16_t harmonic = 1; harmonic <= NUM_HARMONICS; harmonic++)
            {
                int16_t center_bin = round((note_frequencies[n] * octave * harmonic) / divisor_ratio);
                int16_t min_bin = center_bin - (NUM_BINS_TO_SEARCH * harmonic);
                int16_t max_bin = center_bin + (NUM_BINS_TO_SEARCH * harmonic);

                float max_val = 0.0;

                for (int16_t k = min_bin; k < max_bin; k++)
                {
                    if (magnitude_spectrum[k] > max_val)
                        max_val = magnitude_spectrum[k];
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

void calculate_magnitude_spectrum()
{
    magnitude_spectrum = (float*) malloc((BUFFER_SIZE / 2 + 1) * sizeof(float));

    make_hamming_window();

    for (int16_t i = 0; i < BUFFER_SIZE; i++)
        x_sp[2 * i] *= window[i];

    free(window);

    perform_fft();

    for (int16_t n = 0; n < (BUFFER_SIZE / 2) + 1, n++)
        magnitude_spectrum[n] = sqrt(sqrt(y_real_sp[n] * y_real_sp[n] + y_imag_sp[n] * y_imag_sp[n]));
}

void make_hamming_window()
{
    window = (float*) malloc(N * sizeof(float));

    for (int16_t n = 0; n < N; n++)
        window[n] = 0.54 - 0.46 * cos(2 * PI * (((float)n) / ((float) BUFFER_SIZE)));
}

float round(float val)
{
    return floor(val + 0.5);
}