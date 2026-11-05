# Experimental Results and Calculations

## 1. Gate Driver Verification

The gate-driver circuit using the STGAP2SICSNC IC was experimentally tested.

| Parameter | Measured value |
|---|---:|
| Input voltage | 1.68 V |
| Gate-driver output voltage | 6.66 V |
| Switching frequency | 50 kHz |

Similar amplification behaviour was observed at 100 kHz and 120 kHz.

## 2. PWM Generation

PWM signals were generated using the ePWM peripheral of the TMS320F28379D.

| Parameter | 50 kHz | 100 kHz |
|---|---:|---:|
| EPWM clock | 50 MHz | 50 MHz |
| TBPRD | 999 | 500 |
| CMPA | — | 250 |
| Duty cycle | 50% | 50% |

Complementary PWM signals were generated through ePWM1A and ePWM1B, with deadband insertion to prevent simultaneous conduction of the switches.

## 3. Experimental Setup

The switching-characterization experiments were performed on the SCT35N65 SiC MOSFET.

| Parameter | Value |
|---|---:|
| DC bus voltage | 28 V |
| Inductor | 100 µH |
| Capacitor | 100 µF |
| Gate-driver supply | 10 V |
| Drain current (reported measurement) | 0.92 A |

## 4. Switching Energy Measurements

Switching energy was measured using the Yokogawa DLM3054 oscilloscope's IntegTY+ function, which integrates instantaneous power:

\[
E_{\mathrm{sw}}=\int V_{DS}(t)I_D(t)\,dt
\]

### Measured results

| Frequency | Turn-ON energy (µJ) | Turn-OFF energy (µJ) | Total energy (µJ) | Switching loss (W) |
|---:|---:|---:|---:|---:|
| 50 kHz | 57.0940 | 79.8686 | 136.9626 | 6.8481 |
| 100 kHz | 19.5986 | 37.0134 | 56.6120 | 5.6612 |
| 125 kHz | 2.98082 | 2.89938 | 5.88020 | 0.7350 |

Switching power was calculated using:

\[
P_{\mathrm{sw}}=(E_{\mathrm{on}}+E_{\mathrm{off}})f_s
\]

### Manual calculation

Using a triangular approximation:

\[
E_{\mathrm{sw}}\approx\frac{1}{2}\Delta V\Delta I\Delta T
\]

| Parameter | Turn-ON | Turn-OFF |
|---|---:|---:|
| Voltage change | 11.5 V | 11.6 V |
| Current change | 3.04 A | 3.21 A |
| Transition time | 0.93 µs | 3.38 µs |
| Calculated energy | 16.2564 µJ | 62.928 µJ |

Total estimated switching energy: **79.1852 µJ**

Estimated switching loss at 50 kHz: **3.959 W**

## 5. Double Pulse Test

A Double Pulse Test was performed to characterize the switching behaviour of the SiC MOSFET.

- First pulse width: 50 µs
- Second pulse width: 2.4 µs
- Drain-source voltage and drain current were captured using the oscilloscope.

The report's separate DPT calculation section also records switching losses of 6.36475 W at 50 kHz and 12.7295 W at 100 kHz.

## 6. Experimental Observations

- Switching ringing was observed in the drain-source voltage and drain-current waveforms.
- The ringing was attributed to parasitic PCB/bus inductance interacting with the MOSFET output capacitance (\(C_{\mathrm{oss}}\)).
- These oscillations complicate manual switching-energy calculations.
- Oscilloscope integration was therefore used to measure the actual voltage-current overlap.
- Simultaneous measurement of both switches required an isolated probe to avoid shorting their ground references.

## 7. Key Outcomes

- Designed and tested an isolated SiC MOSFET gate-driver circuit.
- Developed a two-layer PCB using Altium Designer.
- Simulated the circuit using LTspice.
- Generated complementary PWM signals with deadband using the TMS320F28379D.
- Experimentally verified gate-driver operation.
- Performed switching characterization and a Double Pulse Test.
- Analysed switching energy, power losses, and parasitic ringing.
