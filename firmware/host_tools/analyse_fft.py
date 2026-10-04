from pathlib import Path
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import butter, sosfiltfilt

CAPTURE_DIRECTORY = Path("capture")
FFT_SIZE = 16384
DISPLAY_MAX_FREQUENCY_HZ = 19000
FILTER_ORDER = 6
FILTER_CUTOFF_HZ = 12000


def main():
    capture_files = sorted(CAPTURE_DIRECTORY.glob("adc_capture_*.npz"))

    if not capture_files:
        raise FileNotFoundError("No capture files found.")

    capture_path = capture_files[-1]
    print(f"Loading: {capture_path}")

    with np.load(capture_path) as capture:
        raw_samples = capture["raw_samples"].astype(np.float64)
        sample_rate_hz = float(capture["sample_rate_hz"])

    sos = butter(FILTER_ORDER, FILTER_CUTOFF_HZ, btype='lowpass', fs=sample_rate_hz, output="sos")

    filtered_samples =sosfiltfilt(sos, raw_samples)

    raw_samples = raw_samples[:FFT_SIZE]
    filtered_samples = filtered_samples[:FFT_SIZE]

    window = np.hanning(FFT_SIZE)

    raw_centred = raw_samples - np.mean(raw_samples)
    filtered_centred = filtered_samples - np.mean(filtered_samples)

    raw_spectrum = np.fft.rfft(raw_centred * window)
    filtered_spectrum = np.fft.rfft(filtered_centred * window)

    frequencies_hz = np.fft.rfftfreq(FFT_SIZE, d=1.0 / sample_rate_hz)

    raw_magnitude = 2.0 * np.abs(raw_spectrum) / np.sum(window)
    filtered_magnitude = (2.0 * np.abs(filtered_spectrum) / np.sum(window))

    search_region = ((frequencies_hz >= 100) & (frequencies_hz <= DISPLAY_MAX_FREQUENCY_HZ))

    raw_peak_index = np.argmax(raw_magnitude[search_region])
    raw_peak_frequency_hz = frequencies_hz[search_region][raw_peak_index]
    raw_peak_magnitude = raw_magnitude[search_region][raw_peak_index]

    filtered_peak_index = np.argmax(filtered_magnitude[search_region])
    filtered_peak_frequency_hz = frequencies_hz[search_region][filtered_peak_index]
    filtered_peak_magnitude = filtered_magnitude[search_region][filtered_peak_index]

    print(f"FFT size: {FFT_SIZE}")
    print(f"Frequency-bin spacing: {sample_rate_hz / FFT_SIZE:.3f} Hz")
    print(f"Peak frequency: {raw_peak_frequency_hz:.3f} Hz")
    print(f"Peak magnitude: {raw_peak_magnitude:.2f} raw-code peak")

    print(f"FFT size: {FFT_SIZE}")
    print(f"Frequency-bin spacing: {sample_rate_hz / FFT_SIZE:.3f} Hz")
    print(f"Peak frequency: {filtered_peak_frequency_hz:.3f} Hz")
    print(f"Peak magnitude: {filtered_peak_magnitude:.2f} raw-code peak")

    '''def peak_near(center_hz, magnitude, half_width_hz=50):
        region = np.abs(frequencies_hz - center_hz) <= half_width_hz

        if not np.any(region):
            raise ValueError(f"No FFT bins near {center_hz:.1f} Hz")

        local_index = np.argmax(magnitude[region])
        local_frequencies = frequencies_hz[region]
        local_magnitudes = magnitude[region]

        return local_frequencies[local_index], local_magnitudes[local_index]


    target_hz = 10_000.0
    mirror_hz = sample_rate_hz / 2 - target_hz

    for label, magnitude in (
        ("Raw", raw_magnitude),
        ("Filtered", filtered_magnitude),
    ):
        f0, a0 = peak_near(target_hz, magnitude)
        fm, am = peak_near(mirror_hz, magnitude)

        print(f"{label}: signal peak = {f0:.3f} Hz, magnitude = {a0:.2f}")
        print(f"{label}: mirror peak = {fm:.3f} Hz, magnitude = {am:.2f}")'''

    plt.figure(figsize=(10, 4))
    plt.plot(frequencies_hz, raw_magnitude, linewidth=0.8, label="Raw")
    plt.plot(frequencies_hz, filtered_magnitude, linewidth = 0.8, label="Filtered")

    plt.legend()

    plt.xlim(0, DISPLAY_MAX_FREQUENCY_HZ)
    plt.xlabel("Frequency (Hz)")
    plt.ylabel("Magnitude (raw-code peak)")
    plt.title("ESP32 ADC spectrum")
    plt.grid(True)

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()










