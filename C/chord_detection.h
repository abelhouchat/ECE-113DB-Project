#ifndef CHORD_DETECTION_H
#define CHORD_DETECTION_H


float norm(float* a);
	/* Calculates the norm of a float vector. */

float cosine_similarity(float* a, float* b);
	/*
	 * Calculates the cosine similarity between two float vectors,
	 * given by <a, b> / (||a|| * ||b||)
	 */

char* find_chord(float* chroma);
	/* Determines which chord a chromagram corresponds to. */


#endif
