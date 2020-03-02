#include "chord_detection.h"
#include "templates.h"

#include <math.h>


float norm(float* a)
{
	/* Calculates the norm of a float vector. */

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
	/*
	 * Calculates the cosine similarity between two float vectors,
	 * given by <a, b> / (||a|| * ||b||).
	 *
	 * First calculates the dot product, then divides by the
	 * product of the norms.
	 */

    float dot = 0;

    int i;
    for (i = 0; i < 12; i++)
    {
    	dot += a[i] * b[i];
    }

    dot /= (norm(a) * NORM);
    return dot;
}

char* find_chord(float* chroma)
{
	/*
	 * Determines which chord a chromagram corresponds to.
	 *
	 * Keeps an array of cosine similarities between the
	 * chromagram and the templates, then finds the maximum
	 * and the corresponding chord.
	 */

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

    return chords[max_index];
}
