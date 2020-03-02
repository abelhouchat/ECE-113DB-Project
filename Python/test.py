import librosa
import librosa.display
import matplotlib.pyplot as plt
import numpy as np
from scipy import signal

y, sr = librosa.load('c1.wav')
y_harmonic = librosa.effects.harmonic(y)
chroma = librosa.feature.chroma_cqt(y = y_harmonic, sr = sr)

plt.figure(figsize = (10, 4))
librosa.display.specshow(chroma, y_axis = 'chroma', x_axis = 'time')
plt.colorbar()
plt.title('Chromagram for C major')
plt.tight_layout()
plt.show()

order = 6
# fc = 64
w = 0.2
b, a = signal.butter(order, w)
for (index, row) in enumerate(chroma):
    filtered = signal.lfilter(b, a, row)
    chroma[index] = filtered

plt.figure(figsize = (10, 4))
librosa.display.specshow(chroma, y_axis = 'chroma', x_axis = 'time')
plt.colorbar()
plt.title('Chromagram')
plt.tight_layout()
plt.show()