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

   pinMode(colour_sensor,INPUT);

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

 
void open_arm(int angle, int angular_velocity, int max_angle){
    if(angle<max_angle){
         for (pos = angle; pos <= max_angle; pos += angular_velocity) {
    myservo.write(pos);            
    delay(15);                       
  }

    }
}

void close_arm(int angle, int angular_velocity){
  if(angle >=0){
    for (pos = angle; pos >= 10; pos -= angular_velocity) { 
    myservo.write(pos);              
    delay(15);                       
  }
    }
  }

void Rotate_left_inplace(double s, double t){
    Motor3->setSpeed(s);
    Motor4->setSpeed(s);
    Motor3->run(BACKWARD);
    Motor4->run(FORWARD);
    delay(t);

}

 void Rotate_right_inplace(double s, double t){
    Motor3->setSpeed(s);
    Motor4->setSpeed(s);
    Motor3->run(FORWARD);
    
    Motor4->run(BACKWARD);
    delay(t);

 }

 void Tilt_right(double s, double t){
     Motor3->setSpeed(s);
    Motor3->run(FORWARD);
    delay(t);
 }

 void Tilt_left(double s, double t){
     Motor4->setSpeed(s);
    Motor4->run(FORWARD);
    delay(t);
 }


void Rotate_left_until_match(){

   int limit_move = 0;

   while (digitalRead(IR2) == LOW | digitalRead(IR1)==HIGH ){

     limit_move++;

     Rotate_left_inplace(100,0);

     if (limit_move > 30){break;}

   }

 

   Brake(100);

}

 

void Rotate_right_until_match(){

   int limit_move = 0;

   while (digitalRead(IR2)== LOW | digitalRead(IR3)==HIGH ){

     limit_move++;

     Rotate_right_inplace(100,0);

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
   Distance_sensor();

 

   while (distance >= max_bound){
       Distance_sensor();

     limit_move++;

     Rotate_right_inplace(100,0);

     if (limit_move > 70){

       break;

       }

     }

 

     Brake(300);

 Distance_sensor();

   while (distance >= reachable_bound){
       Distance_sensor();

        move_forward(100,0);

     }

 

     Brake(100);

     colour_detection();

    //  open_arm(0,3,90);

    //  close_arm(90,3);

    //  task = 1;

    //  line_recovery();

   

}

void color_path(){
    if (colour_hold==1){
        Rotate_left_inplace(200,1600);
        Brake(100);
    };
    if (colour_hold==0){
        Rotate_right_inplace(200,1600);
        Brake(100);
    };
}

void line_follow(){

 

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
    move_forward(230,4000);
    Rotate_left_inplace(200,700);
   line_recovery();  

   }

 

//junction

else if(digitalRead(IR1) == HIGH && digitalRead(IR2) == HIGH  && digitalRead(IR3) == HIGH){
    Distance_sensor();

   junction();

   }


}

 

void line_recovery(){

   

 

   while(digitalRead(IR2)==LOW & digitalRead(IR1)==LOW & digitalRead(IR3)==LOW){

     move_backward(200,100);

     Brake(200);}

}

 

void junction (){

 

 if (task == 0) {

   if (distance >= 30 && distance <= 45)
   {sweep();
      open_arm(0,3,90);
      delay(800);
      close_arm(90,3);
      delay(800);
       Distance_sensor();
       Rotate_left_inplace(200,3150);
       task = 1;
       Distance_sensor();
     }
    else{
    Brake(200);
    move_forward(220,200);
    Distance_sensor();
    }

else if (task == 1) {
    if (distance >= 85 && distance <= 105)
   {move_backward(200,1500);
      Brake(300);
      color_path();
      move_forward(220,3000);
      Distance_sensor();

     }
    else{
    Brake(200);
    move_forward(220,200);
    Distance_sensor();
    }

 

}

}

 
void loop() {

   Distance_sensor();

   line_follow();


}