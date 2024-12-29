# Arduino Nano based PID Line Follower

## Description
This project is a PID-based line-following robot that uses the Arduino Nano microcontroller and a 5-array IR sensor module. The robot detects and follows a white line on a black background with accuracy and speed.

## Features
- PID control for precise line following
- Adjustable speed and tuning parameters
- Compact and efficient design

---

## Table of Contents
1. [Components](#components)
2. [Circuit Diagram](#circuit-diagram)
3. [Setup Instructions](#setup-instructions)
4. [PID Tuning](#pid-tuning)

---

## Components
- Arduino Nano
- TCRT5000L 5-Channel Tracking Sensor Module
- TB6612FNG Motor Driver
- Motors and Wheels
- Chassis
- 7.4V Li-ion Battery (11.1V Li-Po Battery is also fine)

---

## Circuit Diagram

![Circuit Diagram](./Circuit%20Diagram/Line%20Follower%20Circuit.png)

---

## Setup Instructions
1. **Wiring**: Connect the components as per the circuit diagram.
2. **Upload Code**:
   - Open the Arduino IDE.
   - Select `Arduino Nano` as the board.
   - Install the necessary library from the source 'https://github.com/sparkfun/SparkFun_TB6612FNG_Arduino_Library'.
   - Load the `PID_Line_Following_Code.ino` file and upload it.
3. **Calibration**:
   - Press the calibration button (D11) while the robot is over the line.
4. **Run**:
   - Press the run button (D12) to start the bot.

---

## PID Tuning
PID control parameters:
- `Kp` (Proportional gain)
- `Ki` (Integral gain)
- `Kd` (Derivative gain)

To tune:
1. Start with a high `Kp` value. Adjust until the bot starts oscillating.
2. Gradually increase `Kd` to dampen oscillations.
3. Adjust `Ki` to minimize steady-state error.
