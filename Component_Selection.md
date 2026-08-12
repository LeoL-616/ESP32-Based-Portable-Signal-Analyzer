# Component Selection

# AFE

## Operational Amplifier

### Selected Component

MCP6022

### Requirements

- Single-supply operation at 3.3 V
- Rail-to-rail input/output operation preferred
- Sufficient gain-bandwidth product
- Sufficient slew rate
- Low input-referred noise
- Suitable for analog signal conditioning

### Key Specifications

| Parameter           | Requirement                       | MCP6022        |
| ------------------- | --------------------------------: | -------------: |
| Supply Voltage      | 3.3 V operation                   | 2.5–5.5 V      |
| GBW                 | Sufficient for target bandwidth  | 10 MHz typ.    |
| Slew Rate           | Sufficient for target signal      | 7 V / µs typ.    |
| Input/Output Range  | Close to supply rails             | Rail-to-rail   |
| Input Voltage Noise | Low                               |8.7 nV / $\sqrt{\mathrm{Hz}}$ typ.|
| Configuration       | Single/dual                       | Dual           |
| Unity Gain Stable   | Required for ×1 gain              | Yes            |

### Selection Rationale

The MCP6022 was selected because it supports 3.3 V single-supply operation and provides rail-to-rail input and output operation. Its 10 MHz gain-bandwidth product and 7 V/µs slew rate provide sufficient performance for the intended signal-conditioning stages. The dual-op-amp configuration also provides flexibility for implementing multiple amplification and buffering functions within the AFE.


## Protection Diode

### Selected Component

BAT54

### Requirements

- Schottky diode with a low forward voltage
- Sufficient reverse-voltage rating for the intended protection network
- Low reverse leakage current

### Key Specifications

| Parameter                       | Requirement                       | BAT54       |
| ------------------------------- | --------------------------------: | ----------: |
| Forward Voltage Drop            | As low as practical               | 320 mV*     |
| Repetitive Peak Reverse Voltage | Sufficient for the protection network | 30 V    |
| Reverse Leakage Current         | As low as practical               | 1 µA        |

\* At a forward current of 1 mA.

### Selection Rationale

The BAT54 was selected as the protection diode due to its low forward voltage compared with conventional silicon diodes. Its forward voltage is approximately 320 mV at a forward current of 1 mA. Under the simulated operating conditions, the protection network limits the input voltage to approximately -0.32 V to 3.62 V, thereby protecting the subsequent amplifier stage from excessive fault voltages.

The BAT54 also has a repetitive peak reverse voltage rating of 30 V, providing sufficient margin for the intended protection network.


## Input Protection Resistor

### Selected Component

2.2 kΩ resistor

### Selection Rationale

A 2.2 kΩ resistor was selected as the current-limiting resistor in the input protection network. Its primary function is to limit the current flowing through the protection diodes under fault conditions.

For the simulated 5 V fault condition, the current through the protection network was approximately 300 µA. This provides effective current limiting while maintaining a sufficiently high impedance during normal operation.


## Gain-Setting Resistors

### Requirements

The AFE uses a non-inverting amplifier configuration with selectable closed-loop gains of:

- 1×
- 10×
- 100×

The feedback resistor values were selected together with a 1 kΩ lower feedback resistor according to:

$$
A_v = 1+\frac{R_f}{R_g}
$$

where:

$$
R_g = 1\,k\Omega
$$

The selected values are:

| Gain | Feedback resistor $R_f$ | $R_g$ | Theoretical gain |
| ---: | ------------------------: | ------: | ---------------: |
|   1× |                       0 Ω |   1 kΩ |                1 |
|  10× |                     9 kΩ |   1 kΩ |               10 |
| 100× |                    99 kΩ |   1 kΩ |              100 |

The resistor network uses 1% tolerance resistors.


## VBIAS Resistors

### Selected Components

2 × 10 kΩ resistors

### Requirements

A 1.65 V bias voltage is required because the AFE operates from a single 3.3 V supply.

### Selection Rationale

Two 10 kΩ resistors are used to generate the initial VBIAS reference:

$$
V_{BIAS} =3.3\ V \times \frac{10\ k\Omega}{10\ k\Omega + 10\ k\Omega} = 1.65\ V
$$

The generated VBIAS is buffered using an MCP6022 voltage follower before being distributed to the AFE. This prevents the bias network from being significantly affected by the input and amplifier stages.


## Input AC-Coupling Capacitor

### Selected Component

1 × 10 nF capacitor

### Selection Rationale

The 10 nF capacitor provides AC coupling at the AFE input. It blocks the DC component of the incoming signal while allowing the target AC signal to pass to the amplifier stage.


## Gain-Stage High-Pass Capacitor

### Selected Component

1 × 22 µF capacitor

### Selection Rationale

A 22 µF capacitor is connected in series with the lower feedback network of the amplifier. It blocks the DC component of the feedback path, maintaining 1.65 V virtual ground of the system. Together with the 1 kΩ resistor, it establishes the high-pass behaviour of the gain stage.

Its cutoff frequency is approximately:

$$
f_c = \frac{1}{2\pi RC}
$$

with:

$$
R = 1\ k\Omega,\qquad C = 22\ \mu F
$$

giving a cutoff frequency of approximately 7.2 Hz.

This cutoff frequency is sufficiently below the specified lower signal frequency of 10 Hz.


## Output Damping Resistor

### Selected Component

1 × 4.3 Ω resistor

### Selection Rationale

A 4.3 Ω resistor is placed in series with the output low-pass filter to provide damping between the op-amp output and the LC network.

The LC filter can exhibit resonance due to energy exchange between the inductor and capacitor. The series resistor dissipates part of this energy and reduces excessive resonant peaking.

Several resistance values were evaluated through simulation. Increasing the resistance reduced the resonant peak but also introduced greater attenuation near the upper end of the useful frequency range.

The final value was selected as 4.3 Ω

This value provides a suitable compromise between damping the LC resonance and preserving the desired high-frequency response.


## Output Inductor

### Selected Component

1 × 22 µH inductor

### Selection Rationale

A 22 µH inductor is used as the series inductive element of the second-order passive low-pass filter.

The selected value provides the required filtering characteristics while remaining a practical and commercially available component value.

The selected inductor has a tolerance of approximately ±20%, which is considered in the component-tolerance analysis.


## Output Filter Capacitor

### Selected Component

1 × 4.7 µF capacitor

### Selection Rationale

A 4.7 µF capacitor is used as the shunt capacitor in the output LC low-pass filter.

Together with the 22 µH inductor and 4.3 Ω damping resistor, it forms the second-order passive low-pass filter at the AFE output.

The selected filter provides attenuation above the target signal band while maintaining the required signal transmission within the useful frequency range.

The capacitor has an assumed tolerance of ±10%, which is included in the component-tolerance analysis.


## AFE Component Summary

| Component       | Selected Value / Part | Function                          |
| --------------- | --------------------- | --------------------------------- |
| U1/U2           | MCP6022               | Amplification and VBIAS buffering |
| D1/D2           | BAT54                 | Input protection                  |
| $R_{lim}$       | 2.2 kΩ                | Fault-current limiting            |
| $R_g$           | 1 kΩ                  | Gain-setting resistor             |
| $R_f$           | 0 Ω / 9 kΩ / 99 kΩ    | Gain selection                    |
| VBIAS divider   | 10 kΩ / 10 kΩ         | 1.65 V bias generation            |
| $C_1$           | 10 nF                 | Input AC coupling                 |
| $C_2$           | 22 µF                 | Low-frequency feedback filtering  |
| $R_9$           | 4.3 Ω                 | LC filter damping                 |
| $L$             | 22 µH                 | Low-pass filtering                |
| $C$             | 4.7 µF                | Low-pass filtering                |
