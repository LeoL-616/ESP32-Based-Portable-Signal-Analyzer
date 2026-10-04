# ESP32 Acquisition Firmware

## Purpose

This ESP-IDF project runs on a classic ESP32 and transfers sampled AFE output to a host computer. It uses the ESP32 built-in ADC through the I2S peripheral, moves sample blocks with DMA, packages the samples into a binary frame, and sends the frame over UART.

The firmware deliberately transmits raw ADC codes. eFuse Vref calibration was verified during the ADC validation work and remains available for raw-code-to-voltage conversion. The current project stage is concerned with waveform capture and dominant-frequency identification; calibrated voltage and gain measurements are not part of this firmware's acceptance criteria.

For the analog interface, ADC configuration summary, and completed frequency validation, see [`../../ADC/README.md`](../../ADC/README.md).


## Source layout

```text
esp32/
├── CMakeLists.txt             # ESP-IDF project-level build entry point
├── sdkconfig                  # Generated ESP-IDF project configuration
├── main/
│   ├── CMakeLists.txt         # Builds the main firmware component
│   ├── main.c                 # I2S ADC, DMA acquisition, UART transmission
│   ├── adc_protocol.c         # Packet encoding and CRC calculation
│   └── adc_protocol.h         # Packet format and shared constants
└── README.md
```


## Acquisition path

```text
AFE_OUT
   ↓
GPIO34 / ADC1_CH6
   ↓
I2S built-in ADC mode
   ↓
DMA buffer
   ↓
400 raw uint16_t samples
   ↓
packet encoder + CRC
   ↓
UART binary stream
```


## Build and flash

Open the **`esp32`/ project** root in VS Code. It must contain the top-level `CMakeLists.txt` and the `main/` folder; do not open `main` on its own.

In the ESP-IDF extension:

1. Select the target **ESP32** if it is not already selected.
2. Run **ESP-IDF: Build Your Project**.
3. Connect the board and select the correct serial port.
4. Run **ESP-IDF: Flash Your Project**.
5. Use **ESP-IDF: Monitor Your Device** only for startup diagnostics.

Once streaming begins, the UART carries binary packets. A normal serial monitor will display unreadable characters, and it must be closed before the Python receiver opens the same COM port.


## Startup behaviour

The firmware performs the following actions in order:

1. Configures the I2S driver for ESP32 built-in ADC receive mode.
2. Selects ADC1 channel 6 (`GPIO34`) and enables I2S ADC conversion.
3. Installs/configures the UART driver for binary transmission.
4. Repeatedly reads one 400-sample DMA block.
5. Encodes the samples with metadata and a CRC
6. Sends the completed packet through UART.

## Packet ownership

`adc_protocol.h` defines the packet fields used by both the embedded sender and the Python receiver. The packet contains a header, 400 samples, and a CRC. Metadata includes a sequence number, the first sample index, sample count, flags, and nominal/recorded sample-rate information.

Keeping the format in one dedicated module prevents `main.c` from becoming responsible for both hardware acquisition and byte-level serialisation.













