#ifndef TEMPLATES_H
#define TEMPLATES_H

#define NO_CHORD 0.35355339059327376
	/* Constant for the no-chord template, equal to 1 / sqrt(8). */

#define NUM_OF_CHORDS 25
	/*
	 * We have 12 major chords, 12 minor chords, and one no-chord
	 * template.
	 */

#define NORM 1.7320508075688772
	/* Norm of a template vector. */


char chords[NUM_OF_CHORDS][4] = {"C", "C#", "D", "D#", "E", "F", "F#", "G",	"G#", "A", "A#", "B",
								 "Cm", "Cm#", "Dm", "Dm#", "Em", "Fm", "Fm#", "Gm", "Gm#", "Am", "Am#", "Bm",
								 "NC"};

float templates[25][12] = {{1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},	/* Templates corresponding to the above chords. */
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
