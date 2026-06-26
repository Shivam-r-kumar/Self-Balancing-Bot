#include "mpu6500.h"
#include <Wire.h>
#include "motor.h"
#include <AFMotor.h>

#define MOTOR_DIR 1
#define MPU_ADDR 0x68

const int MAX_PWM = 220;     
const int MIN_PWM = 35;     
const float FALL_ANGLE = 65.0;

unsigned long prevTime;
float angle;

MPU6500 mpu(MPU_ADDR);

AF_DCMotor motorLeft(2);
AF_DCMotor motorRight(3);

float targetAngle = 2.0f;

float Kp = 30;
float Ki = 700;
float Kd = 1;

float error = 0.0f;
float integral = 0.0f;
float derivative = 0.0f;
float prevError = 0;

float output = 0.0f;

void setup() {

  Serial.begin(115200);
  mpu.setup();

  stopMotors();

  prevTime = micros();
}

void loop() {

  unsigned long currentTime = micros();
  float dt = (currentTime - prevTime) / 1000000.0;
  if (dt > 0.05) dt = 0.05;
  prevTime = currentTime;

  angle = mpu.readAngle(dt);
  error = targetAngle - angle;

  if (fabs(error) < 5.0f) {
    integral += error * dt;

    integral = constrain(integral, -200.0f, 200.0f);
  } else {
    integral = 0.0f;
  }

  // derivative = -mpu.getGyroRate();

  derivative = (error - prevError) / dt;
  prevError = error;

  output = Kp * error + Ki * integral + Kd * derivative;

  if (fabs(output) < 15) output = 0;

  output = constrain(output, -255.0f, 255.0f);

  if (fabs(angle) >= FALL_ANGLE) {
    driveMotors(0);
    return;
  }

  driveMotors(output);

  Serial.print("Angle -- ");
  Serial.print(angle);
  Serial.print(" -- Pid -- ");
  Serial.println(output);
  delay(10);
  
}


void driveMotors(float pidValue) {
  pidValue *= MOTOR_DIR;

  int pwm = constrain((int)abs(pidValue), 0, MAX_PWM);

  if (pidValue > 0) {
    motorLeft.run(FORWARD);
    motorRight.run(FORWARD);
  } else {
    motorLeft.run(BACKWARD);
    motorRight.run(BACKWARD);
  }

  motorLeft.setSpeed(pwm);
  motorRight.setSpeed(pwm);
}

void stopMotors() {
  motorLeft.setSpeed(0);
  motorRight.setSpeed(0);
  motorLeft.run(RELEASE);
  motorRight.run(RELEASE);
}