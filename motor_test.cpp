#include <Wire.h>

#include <Adafruit_MotorShield.h>

#include "utility/Adafruit_MS_PWMServoDriver.h"

 

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *Motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *Motor4 = AFMS.getMotor(4);

 

void setup() {

  // put your setup code here, to run once:

  AFMS.begin();

}

 

void loop() {

  // put your main code here, to run repeatedly:

  Motor3->setSpeed(150);
  Motor3->run(FORWARD);
  Motor4->setSpeed(150);
  Motor4->run(FORWARD);


}
