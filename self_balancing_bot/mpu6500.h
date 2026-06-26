#ifndef MPU6500_H
#define MPU6500_H

#include <Arduino.h>

class MPU6500 {
private:

  float ay, az;
  float gx;

  float accAngle;
  float gyroAngle;
  float gyroOffsetX;

  uint8_t _addr;
  float accScale;
  float gyroScale;

  float angle = 0;

  void writeRegister(uint8_t reg, uint8_t data);
  int16_t read16Bit(uint8_t reg);
  void calibrateMPU();
  bool readBurst();

public:

  MPU6500(uint8_t addr = 0x68);
  bool setup();
  float readAngle(float dt);
  float getGyroRate();

};




#endif