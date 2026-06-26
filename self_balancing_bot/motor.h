// This is for TB661fNG motor driver 

#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
private:
  uint8_t in1, in2, pwmPin, stdby;

  uint8_t pwmChannel;
  uint32_t pwmFreq;
  uint8_t pwmResolution;

public:
  Motor(uint8_t in1, uint8_t in2, uint8_t pwmPin,
        uint8_t channel = 0,
        uint32_t freq = 20000,
        uint8_t resolution = 8);

  void begin();
  void setSpeed(int speed);
  void brake();
  void stop();
};

#endif