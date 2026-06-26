#include "Arduino.h"
#include "mpu6500.h"
#include <math.h>
#include <Wire.h>

#define MPU_ADDR 0x68

MPU6500::MPU6500(uint8_t addr)
  : _addr(addr), accScale(16384.0f), gyroOffsetX(0.0f),
    gyroScale(131.0f) {}



bool MPU6500::setup() {
  Serial.println("MPU Initializing ....... ");

  Wire.begin();
  Wire.setClock(400000);

  Wire.beginTransmission(0x68);
  Wire.setWireTimeout(3000, true);
  byte error = Wire.endTransmission();

  if (error == 0) {
    Serial.println("MPU FOUND");
  } else {
    Serial.println("MPU NOT FOUND");
    while(1);
  }

  writeRegister(0x6B, 0x00);  // wake up
  writeRegister(0x1B, 0x00);  // gyro ±250 dps
  writeRegister(0x1C, 0x00);  // accel ±2g
  writeRegister(0x1A, 0x03);  // filter

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  calibrateMPU();

  digitalWrite(2, LOW);
  return true;
}

void MPU6500::calibrateMPU() {
  Serial.println("Keep MPU6050 Still...");

  long sum = 0;

  for (int i = 0; i < 1000; i++) {
    sum += read16Bit(0x43);
    delay(2);
  }

  gyroOffsetX = (sum / 1000.0f) / gyroScale;  // / 1000.0 - roundoff / 131.0 - gyro conversion

  Serial.print("Gyro Offset = ");
  Serial.println(gyroOffsetX);
}


int16_t MPU6500::read16Bit(uint8_t reg) {
  Wire.beginTransmission(_addr);
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(_addr, (uint8_t)2);

  if (Wire.available() != 2)
    return 0;

  return (Wire.read() << 8) | Wire.read();
}

void MPU6500::writeRegister(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(_addr);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

bool MPU6500::readBurst() {

  Wire.beginTransmission(_addr);
  Wire.write(0x3B);

  if (Wire.endTransmission(false) != 0)
    return false;

  if (Wire.requestFrom(_addr, (uint8_t)14) != 14)
    return false;

  if (Wire.available() < 14)
    return false;

  Wire.read();  // X_high_data
  Wire.read();  // X_low_data

  ay = (float)((int16_t)((Wire.read() << 8) | Wire.read())) / accScale;
  az = (float)((int16_t)((Wire.read() << 8) | Wire.read())) / accScale;

  Wire.read();  // temp_high_data
  Wire.read();  // temp_low_data

  gx = (float)((int16_t)((Wire.read() << 8) | Wire.read())) / gyroScale;
  Wire.read();  // Y_high_data
  Wire.read();  // Y_low_data
  Wire.read();  // Z_high_data
  Wire.read();  // Z_low_data
  return true;
}


float MPU6500::readAngle(float dt) {

  if (readBurst() == false) return angle;

  // Accelerometer Angle
  accAngle = atan2f(ay, az) * 180.0f / PI;

  // Complemetry Angle

  angle = 0.98f * (angle + (gx - gyroOffsetX) * dt)
          + 0.02f * accAngle;

  return angle;
}

float MPU6500::getGyroRate() {
  return gx - gyroOffsetX;
}
