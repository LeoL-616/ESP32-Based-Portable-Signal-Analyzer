# Component Selection



## 1. Operational Amplifier

### Selected Component

MCP6022

### Requirements

- Single-supply operation at 3.3 V
- Rail-to-rail input/output preferred
- Sufficient gain-bandwidth product
- Sufficient slew rate
- Low input-referred noise
- Suitable for analog signal conditioning

### Key Specifications

| Parameter | Requirement | MCP6022 |
|---|---:|---:|
| Supply Voltage | 3.3 V operation | 2.5–5.5 V |
| GBW | Sufficient for target bandwidth | 10 MHz typ. |
| Slew Rate | Sufficient for target signal | 7 V/µs typ. |
| Input/Output Range | Close to supply rails | Rail-to-rail |
| Input Voltage Noise | Low | 8.7 nV/√Hz typ. |
| Configuration | Single/dual | Dual |
| Unity Gain Stable | Required for ×1 | Yes |

### Selection Rationale

MCP6022 was selected because it supports 3.3 V single-supply operation and provides rail-to-rail input and output operation. Its 10 MHz gain-bandwidth product and 7 V/µs slew rate provide sufficient performance for the intended signal-conditioning stage. The dual-op-amp configuration also provides flexibility for additional buffering or signal-conditioning stages.
