#include <Wire.h>

 

#include <Adafruit_MotorShield.h>

 

#include "utility/Adafruit_MS_PWMServoDriver.h"

#include <Servo.h>

 

Servo myservo;  // create a servo object to control the servo

int pos = 0;    // variable to store the servo position

 

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

 

Adafruit_DCMotor *Motor3 = AFMS.getMotor(3);

Adafruit_DCMotor *Motor4 = AFMS.getMotor(4);

 

int IR1=4; //left    

int IR2=5;

int IR3 = 6;   //right

 

int colour_sensor = 7;

 

int trigPin = 1;

int echoPin = 2;

 

double max_bound = 30;

double reachable_bound = 5;

 

int task = 0; //indicates load is not being carried

int detected = 0; //indicates whether an object is detected

int load_delivered = 0; //tracks whether the block has been delivered

int colour_hold = 2; // 1 for blue, 0 for red

 

int blue_led = 3;

int red_led = 4;

//0 is blue, 1 is red etc

 

long duration; // variable for the duration of sound wave travel

int distance;

 

void setup() {

  // put your setup code here, to run once:

   AFMS.begin();

   pinMode(trigPin, OUTPUT);

   pinMode(echoPin, INPUT);

   pinMode(IR1,INPUT);

   pinMode(IR2,INPUT);

   pinMode(IR3,INPUT);

   pinMode(blue_led,OUTPUT);

   pinMode(red_led,OUTPUT);

   pinMode(color_sensor,INPUT);

   myservo.attach(9);

}

 

//NAVIGATION

void move_forward(double s,double t){

     Motor3->setSpeed(s);

     Motor4->setSpeed(s);

    

     Motor3->run(FORWARD);

     Motor4->run(FORWARD);

 

     delay(t);

}

 

void move_backward(double s,double t){

    Motor3->setSpeed(s);

    Motor4->setSpeed(s);

 

    Motor3->run(BACKWARD);

    Motor4->run(BACKWARD);

 

    delay(t);

}

 

void Brake(double t){

   Motor3->setSpeed(0);

   Motor4->setSpeed(0);

 

   delay(t);

   }

 

void Rotate_right(double s, double t){

    Motor3->setSpeed(s);

    Motor4->setSpeed(s);

    Motor3->run(FORWARD);

   

    Motor4->run(BACKWARD);

    delay(t);

}

 

void Rotate_left(double s, double t){

    Motor3->setSpeed(s);

    Motor4->setSpeed(s);

    Motor3->run(BACKWARD);

    Motor4->run(FORWARD);

    delay(t);

}


void Rotate_left_until_match(){

   int limit_move = 0;

   while (digitalRead(IR2) == LOW | digitalRead(IR1)==HIGH ){

     limit_move++;

     Rotate_left(100,0);

     if (limit_move > 30){break;}

   }

 

   Brake(100);

}

 

void Rotate_right_until_match(){

   int limit_move = 0;

   while (digitalRead(IR2)== LOW | digitalRead(IR3)==HIGH ){

     limit_move++;

     Rotate_right(100,0);

     if (limit_move > 30){break;}

    }

      

    Brake(100);

}

 

//OBJECT HANDLING

void Distance_sensor(){

   digitalWrite(trigPin, LOW);

   delayMicroseconds(2);

   digitalWrite(trigPin, HIGH);

   delayMicroseconds(10);

   digitalWrite(trigPin, LOW);

   duration = pulseIn(echoPin, HIGH);

 

   // Calculating the distance

   distance = duration * 0.034 / 2; // Speed of sound wave divided by 2

}

 

void colour_detection(){

 

    if(digitalRead(colour_sensor)==HIGH){

        colour_hold = 1;

        Serial.println("Blue");

        digitalWrite(blue_led,HIGH);

        digitalWrite(red_led,LOW);

        delay(5000);

        digitalWrite(blue_led,LOW);

    }

 

    else if (digitalRead(colour_sensor)==LOW){

        colour_hold = 0;

        Serial.println("Red");

        digitalWrite(red_led,HIGH);

        digitalWrite(blue_led,LOW);

        delay(5000);

        digitalWrite(red_led,LOW);

    }

}

 

void sweep(){

   int limit_move = 0;

 

   while (distance >= max_bound){

     limit_move++;

     Rotate_right(100,0);

     if (limit_move > 70){

       break;

       }

     }

 

     Brake(300);

 

   while (distance >= reachable_bound){

        move_forward(100,0);

     }

 

     Brake(100);

     colour_detection();

     open_arm(0,3,90);

     close_arm(90,3);

     task = 1;

     line_recovery();

   

}

 

void line_follow(task,distance){

 

//go straight

if(digitalRead(IR1) == LOW  && digitalRead(IR2)==HIGH && digitalRead(IR3)==LOW) {

   move_forward(200,450);

}

 

//deviated right

else if(digitalRead(IR1)== HIGH && (digitalRead(IR2)== LOW || digitalRead(IR2)== HIGH && digitalRead(IR3)==LOW) {

   Rotate_left_until_match();

   }

 

//deviated left

else if(digitalRead(IR1) == LOW  && (digitalRead(IR2)== LOW || digitalRead(IR2)==HIGH)  && digitalRead(IR3)==HIGH) {    

   Rotate_right_until_match();

   }

 

//route recovery

else if(digitalRead(IR1) == LOW && digitalRead(IR2) == LOW  && digitalRead(IR3) == LOW){

   line_recovery();  

   }

 

//junction

else if(digitalRead(IR1) == HIGH && digitalRead(IR2) == HIGH  && digitalRead(IR3) == HIGH){

   junction(task, distance)

   }


}

 

void line_recovery(){

   Rotate_right_until_match();

 

   while(digitalRead(IR2)==LOW){

     move_backward(200,100);

     Brake(200);}

}

 

void junction (task, distance){

 

 if task == 0 {

   if (distance >= 85 && distance <= 105):{

     detected = 1;

     Break;

     }

if task == 1 {

 

}

}

 

void colour_path(){

    if (colour_hold==1){

       Rotate_left(200,2000);

       Brake(100);

     };

    if (colour_hold==0){ 

       Rotate_right(200,2000);

       Brake(100);

    };

}

 

void loop() {

   Distance_sensor();

   line_follow(task, distance);

 

   if detected == 1 {

     sweep();

     open_arm(0,3,90);

     close_arm(90,3);

   }    

 

}