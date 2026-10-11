# Python Host Tools

## Purpose 

These python tools receive binary ADC readings from the ESP32, validate and save captured samples, then display the waveform and frequency-domain spectrum on the host computer.

They are used after the ESP32 acquisition firmware has been built, flashed, and started. The ESP32 send raw ADC samples over UART; these scripts perform the PC-side receiving, storage, plotting, and frequency analysis.


## Files

| Files | Responsibility |
| --- | --- |
| `receive_raw.py` | Opens the serial port, receives and validates binary packets, checks sequence/CRC status, and saves a capture as an NPZ file. |
| `plot_capture.py` | Loads the most recent capture, estimates its dominant frequency automatically, and plots a selected number of waveform cycles. |
| `analyse_fft.py` | Loads the most recent capture and displays its FFT spectrum; it can compare raw and filtered data. |
| `requirements.txt` | Lists the third-party Python packages required by these scripts. |


## Installation

Use a terminal opened in this directory:

```text
python -m pip install -r requirements.txt
```

The required packages are:

```text
numpy
pyserial
matplotlib
scipy
```


## Capture and analysis workflow

Run the scripts from the `host_tools/` directory:

```text
python receive_raw.py
python plot_capture.py
python analyse_fft.py
```

The expected flow is:

```text
                ESP32 UART binary packets
                            ↓
                      receive_raw.py
                            ↓
              packet sequence and CRC checks
                            ↓
            capture/adc_capture_<timestamp>.npz
            ↓                                ↓
      plot_capture.py                 analyse_fft.py
    time-domain waveform            frequency spectrum
automatic dominant frequency    raw/filtered FFT comparison
```

## Capture files

`receive_raw.py` saves a capture in NPZ format. The file contains at least:

| Array/value | Meaning |
| --- | --- |
| `raw_samples` | The received raw ADC sample codes. |
| `sample_rate_hz` | The sampling rate recorded for the capture. |

NPZ is a NumPy archive format. It preserves numerical arrays efficiently and can be loaded again without converting samples to text or losing precision.

## Frequency analysis

`plot_capture.py` searches the configured frequency range for the strongest FFT component and uses that dominant frequency to choose how many samples to show for the requested number of cycles.

`analyse_fft.py` is a verification and diagnostic tool. It is useful for viewing frequency content, comparing the raw and filtered spectra, and when a known generator frequency is being tested-checking for a peak close to that expected frequency.

The automatic dominant-frequency result should be used for normal operation. A known-frequency local search is only used during controlled function-generator validation, particularly when inspecting additional ESP32 ADC spectral peaks.


## Current validation scope

The tools were used successfully with captures from a function generator through the AFE at 1 kHz, 5 kHz, and 10 kHz. They support the current goal of waveform display and dominant-frequency identifications.















