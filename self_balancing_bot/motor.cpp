#include "motor.h"
#include <Arduino.h>

Motor::Motor(uint8_t in1, uint8_t in2, uint8_t pwmPin,
             uint8_t channel,
             uint32_t freq,
             uint8_t resolution) {
  this->in1 = in1;
  this->in2 = in2;
  this->pwmPin = pwmPin;

  this->pwmChannel = channel;
  this->pwmFreq = freq;
  this->pwmResolution = resolution;
}

void Motor::begin() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  
  pinMode(pwmPin, OUTPUT);

  stop();
}

void Motor::setSpeed(int speed) {

  int maxPWM = (1 << pwmResolution) - 1;

  speed = constrain(speed, -maxPWM, maxPWM);

  int pwm = abs(speed);

  if (speed > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(pwmPin, pwm);
  }
  else if (speed < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(pwmPin, pwm);
  }
  else {
    brake();
    return;
  }
}

void Motor::brake() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  analogWrite(pwmPin, 0);
}

void Motor::stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(pwmPin, 0);
}