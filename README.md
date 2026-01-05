# WiiCon Remote

> A wireless motion controller for creative applications, inspired by the Wii Remote.

**⚠️ This project is under active development as part of an academic project.**

## Overview

WiiCon is an open-source, network-connected motion controller built around the ESP32 microcontroller and a BMI160 6-axis IMU (accelerometer + gyroscope). It streams orientation and motion data over the network using the **OSC (Open Sound Control)** protocol, making it ideal for:

- Interactive art installations
- Music and sound design (gesture-controlled synthesis)
- Creative coding (Processing, TouchDesigner, Pure Data)
- Motion capture prototyping
- Game development and research

## Features

### Implemented

- **BMI160 IMU Driver** — Direct I2C communication with the Bosch BMI160 sensor
- **Madgwick AHRS Filter** — Real-time orientation estimation (roll, pitch, yaw)
- **Automatic Calibration** — Gyroscope and accelerometer biases calibration on startup
- **Configurable Axis Mapping** — Remap and invert sensor axes as needed
- **Serial Output** — CSV format for debugging and visualization

### Planned

- [x] Network Manager (captive portal for WiFi configuration)
- [x] OSC data transmission over WiFi
- [ ] Raw and filtered data modes
- [ ] Button inputs
<!-- - [ ] Battery monitoring -->
- [x] LED status indicators
- [x] Low-latency optimizations

## Hardware

| Component    | Description                                     |
| ------------ | ----------------------------------------------- |
| **MCU**      | ESP32-C6 Super Mini (or compatible ESP32 board) |
| **IMU**      | BMI160 6-axis accelerometer/gyroscope           |
| **Protocol** | I2C (400kHz)                                    |
| **Power**    | 3.3V (USB or LiPo battery)                      |

## Project Structure

```
wiicon/
├── wiicon.ino      # Main sketch (setup/loop)
├── config.h        # User configuration (pins, addresses, parameters)
├── bmi160.h        # BMI160 driver declarations
├── bmi160.cpp      # BMI160 driver implementation
├── madgwick.h      # Madgwick AHRS filter declarations
├── madgwick.cpp    # Madgwick AHRS filter implementation
├── AUTHORS         # Project contributors
└── README.md       # This file
```

## Configuration

All user-adjustable settings are in `config.h`:

```cpp
// I2C Pins
const int SDA_PIN = 4;
const int SCL_PIN = 2;

// BMI160 I2C Address (0x68 or 0x69)
const uint8_t BMI160_ADDR = 0x68;

// Calibration samples
const int CALIB_SAMPLES = 200;
```

## OSC Protocol

The device can transmit sensor data via OSC over UDP to a target IP address and port.

## LED Status Indicators

The RGB LED indicates the current status of the device:

| Color / Pattern          | Meaning                                                                    |
| :----------------------- | :------------------------------------------------------------------------- |
| ⚪ **White (Fixed 1s)**   | **Startup.** The system has been turned on.                                |
| 🟡 **Yellow (Blinking)**  | **WiFi Search.** Trying to connect to the configured network.              |
| 🟢 **Green (Fixed 2s)**   | **Success.** Connected to WiFi and sensors calibrated.                     |
| 🔵 **Blue (Flash)**       | **Activity.** Blinks every 2s indicating OSC data transmission.            |
| 🔴 **Red (Blinking)**     | **Sensor Error.** The BMI160 sensor or I2C communication was not detected. |
| 🟣 **Magenta (Blinking)** | **Network Error.** Connection timeout exceeded or WiFi not found.          |

## Authors

- Breno Paz — <brenopaz@ufba.br>
- Eduardo Monteiro — <eduardo.monteiro@ufba.br>
- Laura Marques — <laura.marques@ufba.br>
- Vitor Rizzato — <vitormagalhaes@ufba.br>

## Acknowledgments

This project is developed at **Universidade Federal da Bahia (UFBA)** as part of the final project for the course of **Dispositivos Tecnológicos Práticos II**.

### Third-Party Code

- **BMI160 Driver** — Derived from Intel's BMI160-Arduino library and Jeff Rowberg's I2Cdev library
- **Madgwick AHRS Filter** — Based on the algorithm by Sebastian Madgwick

## License

This project is licensed under the **MIT License**. See individual source files for detailed attribution of derived works.
