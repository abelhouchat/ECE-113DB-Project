#ifndef TEMPLATES_H
#define TEMPLATES_H

// Constant for the no-chord template, equal to 1 / sqrt(8).
#define NO_CHORD 0.35355339059327376

// We have 12 major chords, 12 minor chords, and one no-chord template.
#define NUM_OF_CHORDS 25

// Norm of a template vector/
#define NORM 1.7320508075688772


// All the chords we consider for chord recognition.
char chords[NUM_OF_CHORDS][4] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B",
                                "Cm", "Cm#", "Dm", "Dm#", "Em", "Fm", "Fm#", "Gm", "Gm#", "Am", "Am#", "Bm", "NC"};

// Templates for the chords above, in order.
float templates[25][12] = {{1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
                           {0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0},
                           {0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0},
                           {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0},
                           {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
                           {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0},
                           {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0},
                           {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
                           {1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
                           {0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
                           {0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0},
                           {0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1},
                           {1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
                           {0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
                           {0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0},
                           {0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0},
                           {0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1},
                           {1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0},
                           {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0},
                           {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0},
                           {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1},
                           {1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
                           {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
                           {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
                           {NO_CHORD, NO_CHORD, NO_CHORD, NO_CHORD, NO_CHORD, NO_CHORD, NO_CHORD, NO_CHORD, NO_CHORD, NO_CHORD, NO_CHORD, NO_CHORD}};

#endif
