import librosa

import numpy as np

from scipy import signal
from scipy.linalg import circulant, norm
from scipy.stats import mode
from scipy.ndimage import median_filter

from itertools import groupby


chords = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B',
        'Cm', 'Cm#', 'Dm', 'Dm#', 'Em', 'Fm', 'Fm#', 'Gm', 'Gm#', 'Am', 'Am#', 'Bm', 'NC']

sample_rate = 22050
hop_length = 512

order = 6
fc = 0.2

def lowpass(sig, order, fc):
    b, a = signal.butter(order, fc)
    return signal.lfilter(b, a, sig)

def cossim(a, b):
    return np.dot(a, b) / (norm(a) * norm(b))

def make_templates():
    template_major = np.array([1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0])
    template_minor = np.array([1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0])
    # no_chord = (1 / 3) * np.ones((12, 1))
    no_chord = (1 / np.sqrt(8)) * np.ones((12, 1))

    mat_major = circulant(template_major)
    mat_minor = circulant(template_minor)

    templates = np.concatenate((mat_major, mat_minor, no_chord), axis = 1)

    return templates


def chromagram(audio, filter = True):
    y, sr = librosa.load(audio, sr = sample_rate)
    y_harmonic = y

    chroma = librosa.feature.chroma_cqt(y_harmonic, sr, hop_length = hop_length)

    if filter:
        for (index, row) in enumerate(chroma):
            chroma[index] = lowpass(row, order, fc)

    for i in range(chroma.shape[1]):
        chroma[:, i] /= norm(chroma[:, i])

    return chroma


def estimate_chords(audio):
    chroma = chromagram(audio)
    frames = chroma.shape[1]

    templates = make_templates()

    chord_sequence = []

    for f in np.arange(frames):
        c = chroma[:, f]
        similarity = np.matmul(c, templates)
        chord_sequence.append([np.argmax(similarity)])
    chord_sequence = np.reshape(chord_sequence, frames)

    return chord_sequence


def mode_filtering(sequence, padding = False):
    window = 29
    padding_amount = int((window - 1) / 2)
    length = len(sequence)

    filtered_sequence = 24 * np.ones(length, dtype = int)

    for i in np.arange(padding_amount, length - padding_amount):
        windowed_sequence = sequence[(i - padding_amount):(i + padding_amount + 1)]
        m = mode(windowed_sequence)
        m = m[0][0]
        filtered_sequence[i] = int(m)

    if padding:
        for i in np.arange(padding_amount):
            filtered_sequence[i] = filtered_sequence[padding_amount]
            filtered_sequence[length - i - 1] = filtered_sequence[length - padding_amount - 1]

    return filtered_sequence


def find_chord_sequence(audio):
    chord_nums = estimate_chords(audio)

    filtered_chords = mode_filtering(chord_nums, padding = True)
    # filtered_chords = median_filter(chord_nums, size = 17, mode = 'nearest')

    chord_sequence = [chords[int(f)] for f in filtered_chords]

    return chord_sequence


audio = 'PianoChordsElectric_hundred_sec.wav'
# True chord sequence: C, G, D, A, E, B, F#, C#, Ab, Eb, Bb, F

chord_sequence = find_chord_sequence(audio)
chord_occurrences = [(k, sum(1 for i in g)) for k, g in groupby(chord_sequence)]
current_sample = 0
for c in chord_occurrences:
    print(c[0] + ':', "%.2f" % (current_sample * hop_length / sample_rate), '-', "%.2f" % ((current_sample + c[1]) * hop_length / sample_rate))
    current_sample += c[1]