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
void move_forward(double s,double t){
     Motor3->setSpeed(s);
    Motor3->run(BACKWARD);
    Motor4->setSpeed(s);
    Motor4->run(FORWARD);
    delay(t);
}

void move_backward(double s,double t){
    Motor3->setSpeed(s);
    Motor3->run(FORWARD);
    Motor4->setSpeed(s);
    Motor4->run(BACKWARD);
    delay(t);
  
}
void Brake(double t){
  Motor3->setSpeed(0);
  Motor4->setSpeed(0);
  delay(t);
   }
// s control rotate speed, t control rotate extent theoreitcally
void Rotate_left_inplace(double s, double t){
    Motor3->setSpeed(s);
    Motor3->run(FORWARD);
    Motor4->setSpeed(s);
    Motor4->run(FORWARD);
    delay(t);

}

 void Rotate_right_inplace(double s, double t){
    Motor3->setSpeed(s);
    Motor3->run(BACKWARD);
    Motor4->setSpeed(s);
    Motor4->run(BACKWARD);
    delay(t);

 }
 

void loop() {
  
  move_forward(150,1000);
  
  move_backward(150,1000);
  
  Brake(800);


}
