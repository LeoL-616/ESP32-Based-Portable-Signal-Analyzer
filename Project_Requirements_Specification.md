# Project Requirements Specification



## 1. Introduction


### Project Name

Portable ESP32-Based Signal Acquisition and Analysis Platform


### Project Description

This project aims to design and implement a low-cost portable signal acquisition and analysis platform capable of acquiring analog signals, transmitting sampled data to a computer, and performing signal visualisation and frequency-domain analysis.

The system integrates analog signal conditioning, embedded ADC sampling, and Python-based signal processing.



## 2. Project Objectives

The project objectives are:

1. To design a complete mixed-signal acquisition system.
2. To develop an analog front-end for signal conditioning
3. To implement embedded data acquisition using ESP32
4. To develop Python-based tools for signal visualisation and FFT analysis.
5. To gain practical experience in hardware-software co-design.



## 3. Functional Requirements


### Signal Acquisition

The system shall acquire analog input signals and convert them into digital data using an ADC for subsequent processing.


### Signal Conditioning
The system shall provide appropriate signal conditioning before ADC conversion, including signal amplification, filtering, and voltage level adjustment.



### Data Communication

The system shall transmit sampled data from the microcontroller to a computer.


### Signal Analysis

The system shall provide software tools to:

- Display signals in the time domain.
- Perform frequency-domain analysis using FFT.
- Estimate dominant signal frequencies.



## 4. Performance Requirements

| Parameter | Target |
|---|---|
| Sampling frequency | ≥20 kHz |
| ADC resolution | 12-bit |
| Input frequency range | 10 Hz - 10 kHz |
| Communication | UART/USB |
| Operating voltage | 3.3 V |

  

## 5. Hardware Requirements

The hardware system shall include:

- A microcontroller with ADC capability.
- An analog front end based by an operational amplifier, a low-pass filter, and a bias current circuit.
- Communication interface between hardware and computer.
- A prototype PCB for the final design.



## 6. Software Requirements

The software system shall include:


### Embedded Firmware

The embedded firmware shall:

- Configure ADC sampling.
- Acquire analog data.
- Transmit sampled data.


### Computer software

The software shall:

- Receive data from the microcontroller.
- Display waveform data.
- Perform FFT-based frequency analysis.



## 7. Constraints

The project shall consider the following constraints:

- The development period is limited to approximately nine weeks.
- The project should use low-cost components.



## 8. Testing and Success Criteria

The project will be considered successful if:

1. The system can successfully acquire analog signals.
2. The sampled waveform can be displayed on a computer.
3. The FFT analysis can correctly identify dominant frequency components.
4. A functional hardware prototype is demonstrated.
5. Complete documentation is provided.
