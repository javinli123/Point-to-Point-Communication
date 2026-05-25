# Point-to-Point Optical Communication System

A wireless, line-of-sight communication system that establishes a secure data link between an automated tracking transmitter and a stationary receiver using a directional infrared beam instead of RF or Bluetooth.

## Overview
* **Alignment:** Devices sit on the same horizontal plane within a 90° initial window.
* **Range:** Transmits up to a 10-digit decimal number over at least 10 feet (3 meters).
* **Beam Enclosure:** Hardware is deeply recessed inside a slit to narrow the signal beam for accurate alignment.
* **Medium:** 100% wireless and completely free of RF/Bluetooth radiation.

---

## How It Works
The system runs on a sequential state machine. To reset the link or start a new transmission, the units must be power-cycled.

[Power On] ──> [Transmitter Sweeps 90°] ──> [Link Established] ──> [Enter Keypad Data] ──> [Data Transmitted] ──> [Manual Power Reset]
                                                   │
                                     (If Link Fails / Error Occurs)
                                                   ▼
                                             [Idle Error State]

1. **Power:** Turn on both devices at roughly the same time.
2. **Align:** The transmitter sweeps 90° until it detects the receiver's signal, then locks into place.
3. **Input:** The user types a number (up to 10 digits) on the transmitter's 4x4 keypad.
4. **Send:** The number is converted to binary and transmitted via IR pulses to the receiver display.
5. **Reset:** If any step fails or a transmission finishes, the system idles. Power-cycle both units to restart.

---

## Features
* **Key Design Elements:** Timed IR binary encoding, majority-vote decoding for noise reduction, automated stepper-motor scanning, and a clean modular architecture (Input -> Processing -> Transmission -> Output).
* **User Interface:** 16x2 I2C LCD screens display real-time status updates (e.g., "Searching...").
* **Form Factor:** Standalone 3D-printed enclosures with independent battery power and rocker switches.

---

## Hardware & Components

### Wiring Diagrams
| Transmitter Configuration | Receiver Configuration |
| --- | --- |
| ![Transmitter Wiring](wiring/transmitter_wiring.jpg) | ![Receiver Wiring](wiring/receiver_wiring.jpg) |

### Component Bill of Materials & Cost Breakdown

| Component | Quantity | Estimated Unit Cost | Subtotal |
| --- | :---: | :---: | :---: |
| Arduino Uno Microcontroller Boards | 2 | $27.60 | $55.20 |
| 5V Unipolar Stepper Motor (28BYJ-48) + Driver | 1 | $4.89 | $4.89 |
| 16x2 Character LCD Screen with I2C Module | 2 | $10.49 | $20.98 |
| 4x4 Matrix Membrane Keypad | 1 | $6.85 | $6.85 |
| 38kHz IR Transmitter LED & Receiver pair | 1 | $7.90 | $7.90 |
| 3D Printed Structural Enclosures (PLA Material) | — | $18.50 | $18.50 |
| Hardware & Components (Wago blocks, Rocker switches) | — | $18.09 | $18.09 |
| **Total Estimated System Cost** | | | **$132.41** |

---

## CAD Enclosure Design
Both units share an identical height profile and footprint area to keep the optical link level.

![System Overview](images/system_overview.jpeg)

* `transmitter.png` / `receiver.png`: Enclosures with a narrow vertical slit for beam containment.
* `mount.png`: Automated panning base for the transmitter.
* `lid.png`: Low-profile covers enclosing all internal electronics.

![Transmitter Scanning](images/transmitter_searching.jpeg)

---

## Technical Specs & Performance
* **Electrical:** 5V DC operating voltage. Current draw is 420 mA max during sweeps, dropping to 100 mA at idle.
* **Battery Life:** ~1.38 hours for the transmitter (continuous motor use) and ~5.80 hours for the receiver.
* **Performance:** Range verified up to 3.8 meters (12.5 feet). Alignment takes 0 to 30 seconds.
* **Protocol & Timing:** Custom 34-bit direct-binary protocol operating at 10 Hz. Uses a 200 ms start pulse, followed by 34 data bits (50 ms pulse / 50 ms gap per bit). 
* **Payload & Speed:** Maximum value of 9,999,999,999 sent in exactly 3.6 seconds with a 95% alignment success rate.

---

## Acknowledgements
This project was developed as part of the ENG EK 210 course curriculum at Boston University during the Spring 2026 semester. It represents a collaborative engineering effort executed and finalized by a four-person team.