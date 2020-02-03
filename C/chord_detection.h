#ifndef CHORD_DETECTION_H
#define CHORD_DETECTION_H


float norm(float* a);

float cosine_similarity(float* a, float* b);

int chord_matching(float* chroma);

char* find_chord(float* chroma);


#endif
