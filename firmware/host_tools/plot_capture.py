from pathlib import Path
import numpy as np
import matplotlib.pyplot as plt

CAPTURE_DIRECTORY = Path("capture")
ANALYSIS_FFT_SIZE = 16384
FREQUENCY_SEARCH_LOW_HZ = 100
FREQUENCY_SEARCH_HIGH_HZ = 10500
PLOT_CYCLES = 10


def estimate_dominant_frequency(raw_samples, sample_rate_hz):
    fft_size = min(ANALYSIS_FFT_SIZE, raw_samples.size)

    samples = raw_samples[:fft_size].astype(np.float64)
    samples = samples - np.mean(samples)

    window = np.hanning(fft_size)

    spectrum = np.fft.rfft(samples * window)
    magnitudes = np.abs(spectrum)

    frequencies_hz = np.fft.rfftfreq(fft_size, d=1.0 / sample_rate_hz)

    search_region = ((frequencies_hz >= FREQUENCY_SEARCH_LOW_HZ) & (frequencies_hz <= FREQUENCY_SEARCH_HIGH_HZ))

    peak_index = np.argmax(magnitudes[search_region])

    return frequencies_hz[search_region][peak_index]




def main():
    capture_files = sorted(CAPTURE_DIRECTORY.glob("adc_capture_*.npz"))

    if not capture_files:
        raise FileNotFoundError(f"No capture files found in : {CAPTURE_DIRECTORY}")

    capture_path = capture_files[-1]
    print(f"Loading: {capture_path}")

    with np.load(capture_path) as capture:
        raw_samples = capture["raw_samples"]
        sample_rate_hz = float(capture["sample_rate_hz"])

    dominant_frequency_hz = estimate_dominant_frequency(raw_samples, sample_rate_hz)

    plot_sample_count = round(PLOT_CYCLES * sample_rate_hz /dominant_frequency_hz)

    samples_to_plot = raw_samples[:plot_sample_count]

    time_seconds = (np.arange(samples_to_plot.size) / sample_rate_hz)

    print(f"Dominant frequency: {dominant_frequency_hz:.3f} Hz")
    print(f"Displaying {PLOT_CYCLES} cycles")
    print(f"Displayed samples: {plot_sample_count}")

    print(f"Total samples: {raw_samples.size}")
    print(f"Sample rate: {sample_rate_hz:.3f} Hz")
    print(f"Raw mean: {np.mean(raw_samples):.2f}")
    print(f"Raw minimum: {np.min(raw_samples)}")
    print(f"Raw maximum: {np.max(raw_samples)}")

    zero_indices = np.flatnonzero(raw_samples == 0)
    low_indices = np.flatnonzero(raw_samples < 1800)

    print(f"Zero samples: {zero_indices.size}")
    print(f"Samples below 1800: {low_indices.size}")

    if zero_indices.size > 0:
        print(f"First zero indices: {zero_indices[:10]}")
        print(
            "First zero times (s): "
            f"{zero_indices[:10] / sample_rate_hz}"
        )

    plt.figure(figsize = (10, 4))
    plt.plot(time_seconds, samples_to_plot, linewidth=1.0, marker="o", markersize=2.5,)

    plt.xlabel("Time (s)")
    plt.ylabel("ADC raw code")
    plt.title(f"ESP32 ADC capture: {PLOT_CYCLES} cycles "
              f"at {dominant_frequency_hz:.1f} Hz")
    plt.grid(True)

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main()





