# Analog Front-End (AFE)



## Overview

The analog front-end (AFE) conditions the incoming analog signal before it is sampled by the ESP32 ADC.

The AFE provides:

- AC coupling
- Input protection
- Adjustable gain of 1x, 10x, and 100x
- 1.65 V DC biasing for single-supply operation
- Output low pass filter



## AFE Architecture

The AFE consists of the following stages:

```
Input
  ↓
AC Coupling
  ↓
Input Protection
  ↓
Programmable Gain
  ↓
Output Low-Pass Filter
  ↓
ADC

```



## Input Stage

### AC Coupling

A 10 nF capacitor is used at the AFE input to provide AC coupling.

The capacitor blocks the DC component of the incoming signal while allowing the target AC signal to pass to the amplifier stage.

The input is biased around 1.65 V to allow the AFE to operate from a single 3.3 V supply.

### Input Protection

An input protection network is implemented after the AC-coupling capacitor to protect the amplifier from excessive fault voltages.

The protection network consists of:

- A 2.2 kΩ current-limiting resistor
- BAT54 Schottky diodes
- The 1.65 V VBIAS reference

The resistor limits the current through the protection diodes during a fault condition.

Under a simulated 5 V fault condition, the current through the protection network was approximately 300 µA.

## VBIAS Generation

The AFE operates from a single 3.3 V supply. Therefore, the input signal must be biased around a suitable DC operating point.

A 1.65 V VBIAS reference is generated using a resistor divider:

$$
V_{BIAS} = 3.3\ V \times \frac{10\ k\Omega}{10\ k\Omega+10\ k\Omega} = 1.65\ V
$$

The generated VBIAS is buffered using an MCP6022 voltage follower before being distributed to the AFE.

Buffering the reference prevents the resistor divider from being significantly affected by the amplifier and other connected stages.

## Adjustable Gain Stage

The main amplification stage uses a non-inverting amplifier configuration based on the MCP6022.

Three selectable closed-loop gain settings are provided:

* 1×
* 10×
* 100×

The closed-loop gain is determined by:

$$
A_v = 1+\frac{R_f}{R_g}
$$

where:

$$
R_g=1\ k\Omega
$$

The selected feedback resistors are:

| Gain | $R_f$ | $R_g$ | Theoretical Gain |
| ---: | ----: | ----: | ---------------: |
|   1× |   0 Ω |  1 kΩ |                1 |
|  10× |  9 kΩ |  1 kΩ |               10 |
| 100× | 99 kΩ |  1 kΩ |              100 |

The gain stage is AC-coupled through a 22 µF capacitor in the lower feedback network.

Together with the 1 kΩ resistor, the capacitor establishes a high-pass response with a cutoff frequency of approximately:

$$
f_c =
\frac{1}{2\pi(1\ k\Omega)(22\ \mu F)}
\approx 7.2\ Hz
$$

This places the cutoff frequency below the specified lower signal frequency of 10 Hz.

## Output Low-Pass Filter

A second-order passive LC low-pass filter is implemented at the AFE output.

The filter consists of:

- 4.3 Ω series damping resistor
- 22 µH series inductor
- 4.7 µF shunt capacitor

The filter attenuates higher-frequency components before the signal is passed to the ADC.

### Damping

The 4.3 Ω series resistor provides damping for the LC network and reduces excessive resonant peaking.

Several damping resistance values were evaluated using LTspice. Increasing the resistance reduced the resonance peak but also increased attenuation near the upper end of the target frequency range.

A value of 4.3 Ω was therefore retained as a compromise between damping and preservation of the high-frequency response.

## AFE Design Summary

| Parameter                 |   Selected Value |
| ------------------------- | ---------------: |
| Supply voltage            |            3.3 V |
| VBIAS                     |           1.65 V |
| Input coupling capacitor  |            10 nF |
| Input protection resistor |           2.2 kΩ |
| Gain settings             |  1× / 10× / 100× |
| Gain feedback resistor    | 0 / 9 kΩ / 99 kΩ |
| Gain-stage capacitor      |            22 µF |
| Output damping resistor   |            4.3 Ω |
| Output inductor           |            22 µH |
| Output capacitor          |           4.7 µF |






### AC Coupling

### Input Protection

## Programmable Gain Stage

### Gain = 1

### Gain = 10

### Gain = 100

## Output Low-Pass Filter

## Simulation and Verification

### Gain Verification

### Input Protection

### Frequency Response

### Component Tolerance

## Final AFE Parameters
