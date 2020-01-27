#include "chord_detection.h"

float norm(float* a)
{
    float magnitude = 0;

    for (int16_t i = 0; i < 12; i++)
        magnitude += a[i] * a[i];

    return sqrt(magnitude);
}

float cosine_similarity(float* a, float* b)
{
    float dot = 0;

    for (int16_t i = 0; i < 12; i++)
        dot += a[i] * b[i];
    
    dot /= (norm(a) * norm(b));
    return dot;
}

int16_t chord_matching(float* chroma)
{
    float similarities[NUM_OF_CHORDS];

    float maximum = 0;
    int16_t max_index;
    
    for (int i = 0; i < NUM_OF_CHORDS; i++)
    {
        similarities[i] = cosine_similarity(chroma, templates[i]);
        if (similarities[i] > maximum)
        {
            maximum = similarities[i];
            max_index = i;
        }
    }

    return max_index;
}

char* find_chord(float* chroma)
{
    return chords[chord_matching(chroma)];
}
