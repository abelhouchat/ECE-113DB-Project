#ifndef CHORD_DETECTION_H
#define CHORD_DETECTION_H


// Calculates the norm of a float vector.
float norm(float* a);

/*
 * Calculates the cosine similarity between two float vectors,
 * given by <a, b> / (||a|| * ||b||)
 */
float cosine_similarity(float* a, float* b);

// Determines which chord a chromagram corresponds to.
char* find_chord(float* chroma);


#endif
