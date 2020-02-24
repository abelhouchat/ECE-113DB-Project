#include "chord_detection.h"
#include "templates.h"

#include <math.h>


// Calculates the norm of a float vector.
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

/*
 * Calculates the cosine similarity between two float vectors,
 * given by <a, b> / (||a|| * ||b||)
 */
float cosine_similarity(float* a, float* b)
{
	// Dot product of a and b.
    float dot = 0;

    int i;
    for (i = 0; i < 12; i++)
    {
    	dot += a[i] * b[i];
    }

    dot /= (norm(a) * NORM);
    return dot;
}

// Determines which chord a chromagram corresponds to.
char* find_chord(float* chroma)
{
	/*
	 * Keep array of cosine similarities between chromagram and
	 * templates.
	 */
	float similarities[NUM_OF_CHORDS];

	float maximum = 0;
	int max_index;

	// Find the maximum cosine similarity and its corresponding index.
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
