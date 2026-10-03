# Analog-to-Digital-Converter (ADC)

## Overview

This module connects the analog front end (AFE) to the ESP32 and converts the conditioned analog signal into sample blocks for the host computer. It is the boundary between the analog AFE and the digital acquisition firmware.

The current goal is to capture a waveform and identify its dominant frequency. Accurate amplitude measurement and full frequency-response characterisation are outside of the scope of this stage. Or can be considered as future improvements.


## Interface

The AFE output is connected to the ESP32 as shown in 

| Signal | Connection | Notes |
| --- | --- | --- |
| Analog signal | `AFE_OUT` → ESP32 `GPIO34` | GPIO34 is `ADC1_CH6` and is input_only. |
| Signal range | 0 V to 3.3 V | The ADC input must never exceed the ESP32 supply rails. |
| Signal bias | 1.65 V nominal | The AFE centres the AC signal around the mid-supply bias. |
| Gound | Signal source GND, AFE GND, ESP32 GND | A common ground is required for a valid voltage reference. |
| Supply | 3.3 V | Used by the AFE. |


## Acquisition configuration 

| Parameter | Value | 
| --- | --- |
| MCU | Classic ESP32 |
| ADC unit and channel | ADC1 channel 6 (`GPIO34`) |
| Resolution | 12-bit |
| Attenuation | 11 dB (`attenuation = 3`) |
| ADC mode | ESP32 built-in ADC through I2S receive mode |
| Data transfer | I2S DMA → firmware packet encoder → UART → PC |
| Requested sample rate | 40 kS/s |
| Observed sample rate | 39.694 kS/s |
| Samples per packet | 400 samples |
| Sample payload per packet | 800 bytes (`uint16_t` samples ) |

The ADC is acquired by the ESP32 I2S peripheral in built-in ADC mode. This is why the firmware does not use `adc_oneshot_read()` during normal streaming: I2S and DMA perform the repeated conversions and place the resulting samples into memory.


## Data Path

```text
Known signal source / microphone
               ↓
              AFE
               ↓
    AFE_OUT → GPIO34 (ADC1_CH6)
               ↓
      I2S built-in ADC + DMA 
               ↓
  400-sample binary UART packet
               ↓
Python receiver → NPZ capture file
               ↓
 waveform display and FFT analysis
```


## Verification completed

The system was tested with a sine-wave function generator connected though AFE. Packets were received without observed sequence loss or CRC errors during the validation captures. The python tools successfully saved the captured data, displayed the time-domain waveform, and detected the dominant frequency.

| Generator setting | Detected dominant frequency | Result |
| ---: | ---: | --- |
| 1 kHz | 1000.526 Hz| Pass | 
| 5 kHz | 5000.514 Hz | Pass |
| 10 kHz | 10001.027 Hz | Pass |

The FFT size was 16,384 samples, giving a frequency-bin spacing of approximately 2.423 Hz at the observed sample rate. Each detected frequency is within one FFT bin of the generator setting.

## Known limitations

- The ESP32 ADC spectrum can contain additional peaks. These were recorded during testing but did not prevent the intended 1 kHz, 5 kHz, and 10 kHz signals from being identified.
- The current result demonstrates waveform acquisition and frequency identification. It does not claim calibrated voltage accuracy, AFE gain accuracy, signal-to-noise ratio, or a complete frequency-response measurement.
- The current input is a function generator. Microphone integration will be validated separately after a type and its interface requirements are selected.





