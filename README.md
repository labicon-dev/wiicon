# WiiCon Remote

> A wireless, ESP32-based motion controller for creative applications, inspired by the Wii Remote architecture.

**⚠️ This project is under active development as part of an academic project.**

## Overview

**WiiCon** is an open-source motion controller built around the **ESP32-C6 Super Mini** microcontroller and a **BMI160 6-axis IMU** (accelerometer + gyroscope).

It processes sensor fusion data on-device using a Madgwick AHRS filter and streams orientation (Euler angles) or raw sensor data over WiFi using the **OSC (Open Sound Control)** protocol, making it ideal for:

- Interactive art installations
- Music and sound design (gesture-controlled synthesis)
- Creative coding (Processing, TouchDesigner, Pure Data)
- Motion capture prototyping
- Game development

## Features

- **Dual Mode Data:** Toggle between processed orientation (Roll, Pitch, Yaw) and Raw Sensor Data.
- **WiFi Manager:** Configure WiFi network and OSC target IP address using a web interface.
- **Deep Sleep Support:** Enter deep sleep mode when the button is pressed for 3 seconds.
- **OSC Streaming:** Low-latency UDP transmission of sensor data to a target IP address and port.
- **Visual Feedback:** LED status indicators for startup, connection, activity, and errors.
- **Auto-Calibration:** Automatic calibration of the accelerometer and gyroscope on startup or via button command.

## Hardware

### Components

| Component    | Description                                     |
| :----------- | :---------------------------------------------- |
| **MCU**      | ESP32-C6 Super Mini (or compatible ESP32 board) |
| **IMU**      | Bosch BMI160 6-axis accelerometer/gyroscope     |
| **Feedback** | RGB LED for status indicators                   |
| **Input**    | Tactile Push Button                             |

### Wiring Diagram

| Component Pin  | ESP32 GPIO | Note                      |
| :------------- | :--------- | :------------------------ |
| **BMI160 SDA** | `GPIO 4`   | I2C Data                  |
| **BMI160 SCL** | `GPIO 2`   | I2C Clock                 |
| **Button**     | `GPIO 3`   | Wired to GND (Active Low) |
| **LED Red**    | `GPIO 18`  | 220Ω resistor             |
| **LED Green**  | `GPIO 19`  | 220Ω resistor             |
| **LED Blue**   | `GPIO 20`  | 220Ω resistor             |

## Controls & Interface

The device features a single **multifunciontal button** to control the device without needing a computer.

| Action        | Function          | Description                                                                               |
| :------------ | :---------------- | :---------------------------------------------------------------------------------------- |
| **1 Click**   | **Toggle Mode**   | Switches between sending **Processed Data** (Euler Angles) and **Raw Data** (Accel/Gyro). |
| **2 Clicks**  | **Recalibrate**   | Recalibrates the Gyroscope. **Keep the device still** during the yellow LED blink.        |
| **3 Clicks**  | **Reset WiFi**    | Clears WiFi credentials and restarts into AP Mode (Captive Portal) for reconfiguration.   |
| **Hold (3s)** | **Power (Sleep)** | Enters or Wakes up from **Deep Sleep** mode.                                              |

## LED Status Indicators

The RGB LED provides real-time feedback on the system state:

| Color / Pattern            | Meaning                                                             |
| :------------------------- | :------------------------------------------------------------------ |
| ⚪ **White (Fixed 1s)**     | **Startup.** System is booting up.                                  |
| 🟡 **Yellow (Blinking)**    | **WiFi Search.** Connecting to saved network or AP Mode active.     |
| 🟢 **Green (Fixed 2s)**     | **Ready.** WiFi connected and sensors calibrated successfully.      |
| 🔵 **Blue (Brief Flash)**   | **Heartbeat.** Flashes every 2s indicating active OSC transmission. |
| 🔴 **Red (Fast Blink)**     | **Hardware Error.** BMI160 sensor not detected.                     |
| 🟣 **Magenta (Slow Blink)** | **Network Error.** Connection timeout.                              |


## OSC Protocol

The device transmits data via UDP to the configured target IP (default port: `9000`).

### Address Patterns

- **Euler Angles:** `/wiicon/euler`
  - Arguments: `float roll`, `float pitch`, `float yaw` (Degrees)
- **Raw Accelerometer:** `/wiicon/accel` (Only in Raw Mode)
  - Arguments: `float x`, `float y`, `float z` (g-force)
- **Raw Gyroscope:** `/wiicon/gyro` (Only in Raw Mode)
  - Arguments: `float x`, `float y`, `float z` (deg/s)

## Installation and Configuration

1. **Clone the repo:** `git clone https://github.com/labicon-dev/wiicon.git`
2. **Open in Arduino IDE** (or PlatformIO).
3. **Configure:** Edit `config.h` if your pinout differs.
4. **Upload:** Select **ESP32C6 Dev Module** (or your board) and upload.
5. **First Run:**
    - The LED will blink Yellow.
    - Connect to the WiFi network **"WiiCon Setup"**.
    - A portal will open (or go to `192.168.4.1`).
    - Enter your WiFi credentials and advanced settings if needed.

## Authors

- **Breno Paz** — <brenopaz@ufba.br>
- **Eduardo Monteiro** — <eduardo.monteiro@ufba.br>
- **Laura Marques** — <laura.marques@ufba.br>
- **Vitor Rizzato** — <vitormagalhaes@ufba.br>

### Third-Party Code

- **BMI160 Driver:** Derived from [Intel's BMI160-Arduino library](https://github.com/arduino/ArduinoCore-arc32/blob/master/libraries/CurieIMU/src/BMI160.h) and [Jeff Rowberg's I2Cdev library](https://github.com/jrowberg/i2cdevlib)
- **Madgwick AHRS Filter:** Based on the algorithm by Sebastian Madgwick

## License

This project is developed at **Universidade Federal da Bahia (UFBA)** as part of the final project for the course of **Dispositivos Tecnológicos Práticos II**.

This project is licensed under the **MIT License**. See individual source files for detailed attribution of derived works.
