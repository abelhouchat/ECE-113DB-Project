# ECE-113DB-Project
This is the repository for my final project for ECE 113DB at UCLA.

## Overview
This is a real-time chord recognition system based on [Adam Stark's implementation](https://github.com/adamstark/Chord-Detector-and-Chromagram).
The actual program is written in C, while the prototype was written in Python.

## Python
The Python files are not real-time; they take in an audio file and process the whole thing, outputting the chord sequence.
We have two Python files: `test.py` and `chord_recognition.py`.
`test.py` reads in an audio file and plots the chromagram over time once with no post-processing and then again after applying a low-pass filter to the chromagram.
This gets rid of high-frequency jitters and noise in the chromagram.
Currently we use an order 6 Butterworth filter, but it can be modified to any appropriate filter, order, and cutoff frequency.

`chord_recognition.py` reads in an audio file and outputs the chord sequence to console.
The chord recognition algorithm is similar to the C program, but with some changes and fancy additions since we aren't constrained by real-time.
Notably, we use [librosa](https://github.com/librosa/librosa) to perform chromagram extraction instead of using Adam Stark's algorithm.
We also low-pass the chromagram and apply mode filtering to the chord sequence to further reduce high-frequency noise.

The following libraries are required to run the Python scripts:
* [NumPy](https://github.com/numpy/numpy)
* [SciPy](https://github.com/scipy/scipy)
* [librosa](https://github.com/librosa/librosa)

## C
The C program performs real-time chord recognition on an LCDK using microphone input.
The program collects an audio sample of a certain length, calculates the chromagram, and uses template matching to match it to one of 24 chords.
We stuck to 24 for simplicity, but this could easily be extended to include many others.
To help the user, the program also has a makeshift metronome that beeps when the chord should be played.
Playing audio out of sync with the metronome will cause the program to perform very poorly.

The length of the audio sample is determined by the sampling rate and the `FRAME_SIZE` parameter in `chromagram.h`.
You can change this to include more or less audio per frame, but be aware that the `BUFFER_SIZE` parameter must be one-fourth of `FRAME_SIZE` and `BUFFER_SIZE` should be a power of 2 for the algorithm to work properly.
Furthermore, if the frame is too short, then chord recognition will perform poorly.
Because the program processes audio in discrete frames, the time resolution is limited.

By default, the program runs for a set number of iterations, with each iteration being roughly the length of an audio frame (with some overhead due to chromagram calculation).
After looping for the set number of iterations, the program will generate a text file `chord_sequence.txt` with the timestamped chord sequence.
You can change the number of iterations to whatever is appropriate for however long you want to run chord recognition for.