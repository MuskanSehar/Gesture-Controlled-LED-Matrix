# Gesture-Controlled LED Matrix

An ESP32 project that started as a tilt-controlled dot on an 8x8 LED matrix and evolved into a physics-based maze game — built and tested entirely in Wokwi without any physical hardware.

My goal wasn't just to make something that works. It was to understand how sensors, microcontrollers, and displays communicate at the protocol level, and to learn embedded development workflow through simulation before touching real hardware.

---

## How it evolved

Each commit builds directly on the last. The repository is designed to show learning over time, not just a final result.

| Commit | What was built | What was learned |
|---|---|---|
| 1 | Dot control on 8x8 matrix | I2C, SPI, raw sensor mapping |
| 2 | Expanded to 32x8 across 4 chained panels | Lerp smoothing, float vs int, panel addressing |
| 3 | Ball physics — momentum and friction | Velocity, acceleration, physics simulation |
| 4 | Maze game with real-time collision detection | 2D arrays, collision detection, millis() timing |

---

## What it does (current state)

- Reads X and Y acceleration from the MPU6050 at ~20fps
- Applies physics — tilt adds force, friction decelerates the ball
- Navigates a hardcoded 32x8 maze with real-time wall collision
- Blinking goal marks the target — reach it to trigger a win animation and reset

---

## Built with

- ESP32 DevKit C
- MPU6050 — 6-axis IMU, communicates over I2C
- MAX7219 + 32x8 LED Matrix — 4 chained 8x8 panels driven over SPI
- Arduino C++ — Wire.h, MPU6050.h, MD_MAX72XX.h
- [Wokwi](https://wokwi.com) — browser-based ESP32 simulator

---

## Project structure

```
Gesture-Controlled-LED-Matrix/
├── src/
│   └── main.ino          # Main firmware — sensor read, physics, maze, display
├── wokwi/
│   ├── diagram.json      # Circuit definition for Wokwi simulator
│   └── libraries.txt     # Required libraries
└── docs/
```

---

## Simulation

Open [Wokwi](https://wokwi.com), create a new ESP32 project, paste `src/main.ino` and `wokwi/diagram.json`, add the two libraries, and hit Play. Click the MPU6050 chip to open tilt sliders and navigate the maze.

**Libraries needed:**
- MPU6050 by Electronic Cats
- MD_MAX72XX by majicDesigns

---

## What this was actually for

- Understand how I2C and SPI work in practice — not just in theory
- Learn how raw accelerometer data gets processed into useful outputs
- Build a physics simulation from scratch — velocity, friction, collision
- Get comfortable with Wokwi as a development and debugging environment
- Practice iterative embedded development — each commit adds one new concept

---

## What simulation taught me

Wokwi was the right tool for learning protocols and logic, but it exposed some real limitations worth documenting:

**Sensor calibration** — the simulator's MPU6050 outputs different raw ranges than real hardware. The `map()` function had to be tuned by reading serial output and adjusting bounds manually rather than trusting the datasheet.

**Simulation vs physical interaction** — the maze uses physics-based controls designed for real hardware where you tilt a board naturally with both axes at once. In Wokwi, X and Y are separate sliders — impossible to control simultaneously. Same code, completely different experience. This is why embedded projects need physical testing, not just simulation.

**Panel addressing** — chaining four 8x8 matrices into a 32x8 display required manually calculating device index and bit position for every pixel. The library doesn't treat chained panels as one continuous grid, so `setPoint()` had to be replaced with `setRow()` and bitwise operations.

**Timing** — using `delay()` blocks the entire loop, breaking independent animations. Switching to `millis()`-based timing allowed the goal to blink independently of ball movement — a pattern used in almost every real embedded project.

---

## Planned improvements

- Complementary filter for smoother, drift-free motion
- Snake game — tilt to steer, grow on food, die on collision
- Game selector — flip board upside down (Z axis) to switch between games
- Random maze generation using recursive backtracker algorithm
- WiFi data logging to a web dashboard
