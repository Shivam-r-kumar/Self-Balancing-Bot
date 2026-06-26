# 🤖 Self-Balancing Robot

A simple two-wheel self-balancing robot built using **Arduino Uno**, **MPU6050**, and the **Arduino Motor Driver Shield (L293D)**. The robot estimates its tilt angle using a **Complementary Filter** and balances itself using a **PID Controller**.

> **Note:** This project was originally developed for **ESP32 + TB6612FNG**. I had already written a separate TB6612FNG motor driver library for it. Unfortunately, my ESP32 board and TB6612FNG driver stopped working during development, so I switched to an **Arduino Uno with the Arduino Motor Driver Shield** to complete the project. The repository still contains the TB6612FNG motor driver code, so if you're using ESP32 and TB6612FNG, you can use that implementation instead.

---

# 📌 Features

* MPU6050 angle estimation using a Complementary Filter
* PID-based balancing algorithm
* Modular MPU6050 driver
* Separate motor driver implementation
* TB6612FNG motor driver library included
* Arduino Motor Shield implementation
* Beginner-friendly code structure

---

# 🛠 Hardware Used

* Arduino Uno
* MPU6050 IMU
* Arduino Motor Driver Shield (L293D)
* 2 DC Gear Motors
* Two Wheels
* Battery Pack

---

# 🔌 Wiring

### MPU6050

| MPU6050 | Arduino Uno |
| ------- | ----------- |
| VCC     | 5V          |
| GND     | GND         |
| SDA     | A4          |
| SCL     | A5          |

The Arduino Motor Driver Shield is mounted directly on top of the Arduino Uno, so the motor connections are handled by the shield.

---

# ⚙️ How It Works

The robot continuously reads the accelerometer and gyroscope values from the MPU6050.
The accelerometer provides a stable angle but is noisy.
The gyroscope provides smooth angular velocity but slowly drifts over time.
To combine the advantages of both sensors, a **Complementary Filter** is used:

```cpp
angle = 0.98 * (angle + gyro * dt)
      + 0.02 * accelAngle;
```

This produces a smooth and stable angle estimate.
The calculated angle is then passed to the PID controller.

---

# 🎯 PID Controller

The PID controller continuously calculates the motor output using

```
Output =
Kp × Error
+ Ki × Integral
+ Kd × Derivative
```

where:
```
Error = Target Angle - Current Angle
```

The calculated PID output is converted into motor speed.
Positive output moves the robot in one direction.
Negative output moves it in the opposite direction.
When the robot reaches the target angle, the motors slow down automatically.

---

# 🔧 PID Tuning

Finding the correct PID values is the most important part of building a balancing robot.

### Step 1

Set
```
Ki = 0
Kd = 0
```

Increase **Kp** slowly until the robot starts trying to balance.

---

### Step 2

Increase **Kd** until oscillations become smaller and the robot becomes smoother.

---

### Step 3

Add a small **Ki** value to remove any steady balancing error.
Too much Ki will cause slow oscillations.

---

My final values were

```
Kp = 30
Ki = 700
Kd = 1
```

These values worked well for my robot but may need adjustment depending on your motors, battery voltage, wheel size, and robot weight.

---

# 🧠 Code Structure

```
self_balancing_bot.ino
│
├── Reads MPU6050
├── Calculates angle
├── Runs PID controller
├── Sends motor output
└── Stops motors if robot falls
```

```
mpu6500.cpp / .h
```

Contains

* MPU initialization
* Gyroscope calibration
* Complementary Filter
* Angle calculation

```
motor.cpp / .h
```

Contains a reusable TB6612FNG motor driver implementation for ESP32.
Although this project uses the Arduino Motor Driver Shield, the TB6612FNG driver has been included for anyone using ESP32.

---

# 🛡 Safety

If the robot tilts more than **65°**, the motors are immediately stopped to prevent damage.

---

# 🚀 Future Improvements

* Position PID
* Bluetooth Control
* Remote Tuning
* Better Sensor Fusion
* Kalman Filter
* STM32 Version
* ESP32 Version

---

# 📖 Learning Goals

This project was built to better understand

* Embedded Systems
* Sensor Fusion
* PID Control
* Motor Control
* Real-Time Programming
* IMU Data Processing

---

# 📜 License

This project is open-source. Feel free to use, modify, and improve it for your own projects.

If this repository helps you, consider giving it a ⭐.
