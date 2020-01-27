#ifndef CHORD_DETECTION_H
#define CHORD_DETECTION_H

#include "templates.h"
#include <math.h>

float norm(float* a);

float cosine_similarity(float* a, float* b);

int16_t chord_matching(float* chroma);

char* find_chord(float* chroma);

#endif