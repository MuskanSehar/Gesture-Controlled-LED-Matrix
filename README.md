# Gesture-Controlled LED Matrix

An ESP32 project that reads tilt data from an MPU6050 accelerometer and moves a dot across an 8x8 LED matrix in real time. Developed and tested in Wokwi (ESP32 simulator), so no physical hardware was required.

This was one of my first embedded systems projects. My goal wasn't just to make a moving dot—it was to understand how sensors, microcontrollers, and displays communicate, and to become familiar with hardware simulation as a development workflow.

---

## What it does

- Reads X and Y acceleration from the MPU6050 at ~20fps
- Maps tilt angle to a grid coordinate (0–7 on each axis)
- Renders a moving dot on the MAX7219-driven 8x8 LED matrix
- Prints live sensor values to the serial monitor for debugging

---

## Built with

- ESP32 DevKit C
- MPU6050 — 6-axis IMU, communicates over I2C
- MAX7219 + 8x8 LED Matrix — driven over SPI
- Arduino C++ — Wire.h, MPU6050.h, MD_MAX72XX.h
- [Wokwi](https://wokwi.com) — browser-based ESP32 simulator

---

## Project structure

```
Gesture-Controlled-LED-Matrix/
├── src/
│   └── main.ino          # Main firmware — sensor read, mapping, display
├── wokwi/
│   ├── diagram.json      # Circuit definition for Wokwi simulator
│   └── libraries.txt     # Required libraries
└── docs/
```

---

## Simulation

Open [Wokwi](https://wokwi.com), create a new ESP32 project, paste `src/main.ino` and `wokwi/diagram.json`, add the two libraries, and hit Play. Click the MPU6050 chip to open tilt sliders and control the dot in real time.

**Libraries needed:**
- MPU6050 by Electronic Cats
- MD_MAX72XX by majicDesigns

---

## What this was actually for

- Explore hardware simulation using Wokwi before working with physical hardware
- Learn how an ESP32 communicates with peripherals using I2C and SPI
- Understand how raw accelerometer readings can be processed into useful outputs
- Practice building and debugging embedded systems in a simulated environment
- Gain confidence with embedded C++ and Arduino libraries


---

## Planned improvements

- Complementary filter for smoother, drift-free motion
- Trail effect — last 3 positions stay lit
- Tilt-controlled snake game
- WiFi data logging to a web dashboard


