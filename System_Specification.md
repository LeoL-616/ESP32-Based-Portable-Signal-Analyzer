# System Specification


## 1. Signal Input Specification

The system shall support external analog input signals with different amplitude levels, including computer-generated test signals and sensor outputs.

The target input signal range is:

| Parameter | Target |
|---|---|
| Signal amplitude | 5 mVpp - 1 Vpp |
| Frequency range | 10 Hz - 10 kHz |


The input signals will be conditioned by the analog front-end before ADC conversion.



## 2. Analog Front-End Specification

The analog front-end shall provide signal conditioning functions including:

- Signal amplification.
- Noise filtering.
- Voltage level adjustment for ADC compatibility.


### Adjustable Gain

The system shall provide configurable gain settings to accommodate different input signal amplitudes.

Target gain settings:

| Mode | Gain |
|---|---|
| Low gain | ×1 |
| Medium gain | ×10 |
| High gain | ×100 |



## 3. ADC and Sampling Specification

The system shall digitise conditioned analog signals using the ESP32 ADC.

Target specifications:

| Parameter | Target |
|---|---|
| ADC resolution | 12-bit |
| Sampling frequency | 40 kHz |
| ADC input range | 0 - 3.3V |



## 4. Communication Specification

The ESP32 shall transmit sampled data to a computer through a serial communication interface.

Target:

| Parameter | Target |
|---|---|
| Interface | UART over USB |
| Baud rate | 115200 bps |



## 5. Software Specification

The Python-based software shall provide:

- Time-domain waveform visualisation.
- Frequency-domain analysis using FFT.
- Dominant frequency estimation.
- Optional data storage for offline analysis.


## 6. Performance Targets

The system should:

- Correctly identify known test signals such as 1kHz sine waves.
- Demonstrate real-world signal analysis using microphone input.
- Provide a complete hardware and software demonstration.
