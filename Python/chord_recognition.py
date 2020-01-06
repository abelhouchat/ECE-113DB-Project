import librosa
import numpy as np
from scipy.linalg import circulant
from scipy.stats import mode


chords = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B',
        'Cm', 'Cm#', 'Dm', 'Dm#', 'Em', 'Fm', 'Fm#', 'Gm', 'Gm#', 'Am', 'Am#', 'Bm', 'NC']


def make_templates():
    template_major = np.array([1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0])
    template_minor = np.array([1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0])
    no_chord = (1 / 3) * np.ones((12, 1))

    mat_major = circulant(template_major)
    mat_minor = circulant(template_minor)

    templates = np.concatenate((mat_major, mat_minor, no_chord), axis = 1)

    return templates


def chromagram(audio):
    y, sr = librosa.load(audio)
    y_harmonic, y_percussive = librosa.effects.hpss(y)

    chroma = librosa.feature.chroma_cqt(y_harmonic, sr)

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
    
    return chord_sequence


def mode_filtering(sequence, padding = False):
    window = 15
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

    chord_sequence = [chords[f] for f in filtered_chords]

    return chord_sequence


audio = 'PianoChordsElectric_first_fifty_sec.wav'

chord_sequence = find_chord_sequence(audio)
print(chord_sequence)