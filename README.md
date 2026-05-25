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

* **Standard Flow:** [Power On] -> [Transmitter Sweeps 90°] -> [Link Established] -> [Enter Keypad Data] -> [Data Transmitted] -> [Manual Power Reset]
* **Error Handling:** If a link failure or operational error occurs at any stage, the system enters a protective [Idle Error State] until manually power-cycled.

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
| ![Transmitter Wiring](wiring/transmitter_wiring.png) | ![Receiver Wiring](wiring/receiver_wiring.png) |

### Component Bill of Materials & Cost Breakdown

| Component | Quantity | Estimated Unit Cost | Subtotal |
| --- | :---: | :---: | :---: |
| Arduino Uno Rev3 Microcontroller Boards | 2 | $27.60 | $55.20 |
| LCD Screen 16x2 with I2C Adapter | 2 | $9.99 | $19.98 |
| Overture PLA Filament Spool | 1 | $13.98 | $13.98 |
| M14178 IR 38kHz Transmitter + Receiver Set | 1 | $7.99 | $7.99 |
| 16-Button Membrane Keypad | 1 | $7.55 | $7.55 |
| M3 Screws & Bolts Assortment Kit | 1 | $6.98 | $6.98 |
| ON/Off Rocker Switch | 1 | $6.99 | $6.99 |
| Energizer MAX 9V Batteries (2 Pack) | 1 | $6.78 | $6.78 |
| 5V Stepper Motor (28BYJ-48) + Driver | 1 | $2.96 | $2.96 |
| 5-Line COMPACT Splicing Wago Connectors | 4 | $0.60 | $2.40 |
| Inline Splicing Wago Connectors with Lever | 4 | $0.40 | $1.60 |
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
* **Electrical & Power:** 5V DC operating voltage. Both units draw a baseline 102 mA (0.51W) during standard operation (typing and idling). The transmitter draws a brief 422 mA peak (2.11W) exclusively while the motor runs during the initial alignment sweep.
* **Battery Life:** ~5.68 hours for the receiver, and up to ~5.68 hours for the transmitter during standard typing and transmission. Transmitter operational life scales down to ~1.37 hours if forced into a continuous, non-stop motor-scanning loop.
* **Performance:** Range verified up to 3.8 meters (12.5 feet). Automated spatial alignment completes within a 0 to 30 second window.
* **Protocol & Timing:** Custom 34-bit direct-binary protocol operating at 10 Hz. Uses a 200 ms start pulse, followed by 34 data bits (50 ms pulse / 50 ms gap per bit). 
* **Payload & Speed:** Maximum value of 9,999,999,999 sent in exactly 3.6 seconds with a 95% alignment success rate and a 100% data transmission accuracy rate once locked.

---

## Acknowledgements
This project was developed as a collaborative effort by a four-person team for the ENG EK 210 course curriculum at Boston University during the Spring 2026 semester.