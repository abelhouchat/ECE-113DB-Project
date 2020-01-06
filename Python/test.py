import librosa
import librosa.display
import matplotlib.pyplot as plt

y, sr = librosa.load('piano_major_C.wav')
y_harmonic, y_percussive = librosa.effects.hpss(y)
chroma = librosa.feature.chroma_cqt(y = y_harmonic, sr = sr)
plt.figure(figsize = (10, 4))
librosa.display.specshow(chroma, y_axis = 'chroma', x_axis = 'time')
plt.colorbar()
plt.title('Chromagram')
plt.tight_layout()
plt.show()