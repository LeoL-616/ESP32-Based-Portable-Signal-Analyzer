# System Architecture Design



## 1. Architecture Diagram







## 2. Hardware Architecture


The hardware system consists of three main modules:

1. Analog Front-End (AFE)
2. Embedded Data Acquisition System


### 2.1 Analog Front-End (AFE)

The analog front-end is responsible for preparing input analog signals before digital conversion.

The AFE consists of several functional blocks:


#### Input Protection

The input protection circuit prevents excessive voltage levels from damaging downstream components, particularly the ADC input of the microcontroller.


#### Signal Amplification

The amplification stage increases the amplitude of low-level input signals to improve ADC utilisation.

The gain should be adjustable depending on the input signal amplitude.


#### Signal Filtering

The filtering stage limits the bandwidth of the input signal and reduces unwanted high-frequency noise.

A low-pass filter is implemented to reduce aliasing before ADC sampling.


#### Voltage Level Adjustment

Since the ESP32 ADC operates within a limited input voltage range, the input signal level is adjusted to match the ADC requirements.

For AC signals, a DC bias voltage may be introduced to shift the signal into the ADC input range.



### 2.2 Embedded Data Acquisition System

The embedded acquisition system is based on an ESP32 microcontroller.

The ESP32 performs the following functions:

- Sampling conditioned analog signals through its ADC.
- Temporarily storing sampled data.
- Transmitting digital data to a computer through a serial communication interface.


The embedded system provides the connection between the analog hardware and computer-based signal processing.


| Module | Function |
|---|---|
| Analog Front-End | Signal conditioning before ADC conversion |
| ESP32 | ADC sampling and data transmission |
| Power Management | Provide stable power supply |



## 3. Software Architecture

The software system consists of two main components:

1. Embedded firmware running on the ESP32 microcontroller.
2. Computer-based signal processing software developed using Python.

The software architecture is designed to enable reliable data acquisition, communication, and signal analysis.



## 3.1 Embedded Firmware Architecture

The embedded firmware is responsible for controlling the data acquisition process and transmitting sampled data to the computer.

The main functions include:


### ADC Sampling Control

The firmware configures and controls the ADC sampling process.

The sampling module is responsible for:

- Setting the sampling frequency.
- Reading analog values from the ADC.
- Generating digital samples for further processing.


### Data Buffering

Sampled ADC data is temporarily stored in memory before transmission.

Buffering helps to:

- Maintain a stable data flow.
- Reduce communication interruptions during sampling.


### Serial Communication

The firmware transmits acquired data to the computer through a serial communication interface.

The communication module is responsible for:

- Formatting sampled data.
- Sending data packets.
- Maintaining reliable data transfer.



## 3.2 Computer-Based Software Architecture

The computer-side software is developed using Python and performs data processing, visualisation, and frequency-domain analysis.

The software consists of the following modules:


### Data Acquisition Module

The data acquisition module receives sampled data from the ESP32 through serial communication.

Functions include:

- Establishing communication with the device.
- Receiving incoming data streams.
- Converting received data into usable numerical data.


### Data Processing Module

The processing module prepares acquired data for analysis.

Functions include:

- Data scaling.
- Noise filtering (if required).
- Preparing data for visualisation and FFT analysis.


### Visualisation Module

The visualisation module displays acquired signals in the time domain.

The module provides:

- Real-time waveform plotting.
- Signal amplitude observation.


### Frequency Analysis Module

The frequency analysis module performs FFT-based analysis to extract frequency-domain information.

Functions include:

- Transforming time-domain signals into frequency-domain representations.
- Identifying dominant frequency components.
- Estimating signal frequencies.


## 5. Data Flow

## 6. Design Considerations
