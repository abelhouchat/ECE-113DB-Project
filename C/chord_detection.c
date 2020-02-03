#include "chord_detection.h"
#include "templates.h"

#include <math.h>


float norm(float* a)
{
    float magnitude = 0;

    int i;
    for (i = 0; i < 12; i++)
    {
    	magnitude += a[i] * a[i];
    }

    return sqrt(magnitude);
}

float cosine_similarity(float* a, float* b)
{
    float dot = 0;

    int i;
    for (i = 0; i < 12; i++)
    {
    	dot += a[i] * b[i];
    }

    dot /= (norm(a) * NORM);
    return dot;
}

int chord_matching(float* chroma)
{
    float similarities[NUM_OF_CHORDS];

    float maximum = 0;
    int max_index;

    int i;
    for (i = 0; i < NUM_OF_CHORDS; i++)
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
