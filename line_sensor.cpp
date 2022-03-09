#include <Wire.h>

#include <Adafruit_MotorShield.h>

#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <Servo.h>
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position


  

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *Motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *Motor4 = AFMS.getMotor(4);

int IR1=4;      
int IR2=5;
int IR3 = 6;   

int color_sensor = 5;

int trigPin = 1;
int echoPin = 2; 

double max_bound = 30;
double reachable_bound = 5;

int task = 0;
int load_delivered = 0;
int color_hold = 2;
// 1 for blue, 0 for red

int blue_led = 3;
int red_led = 4;
//0 is blue, 1 is red etc

// 0 is going to collection rea, 1 is delivering package, 2 is going back to initial area

long duration; // variable for the duration of sound wave travel
int distance;



void setup() 
{ AFMS.begin();
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); 
  pinMode(IR1,INPUT);
  pinMode(IR2,INPUT);
  pinMode(IR3,INPUT);
  pinMode(blue_led,OUTPUT);
  pinMode(red_led,OUTPUT);
  pinMode(color_sensor,INPUT);
  myservo.attach(9);
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
    for (pos = angle; pos >= 0; pos -= angular_velocity) { 
    myservo.write(pos);              
    delay(15);                       
  }
    }
  }

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
// s control rotate speed, t control rotate extent theoreitcally
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
 void color_detection(){
    Serial.println(digitalRead(color_sensor));
    if(digitalRead(color_sensor)==HIGH){
        color_hold = 1;

        Serial.println("Blue");
        digitalWrite(blue_led,HIGH);
        digitalWrite(red_led,LOW);
        delay(5000);
        digitalWrite(blue_led,LOW);
    }
    else if (digitalRead(color_sensor)==LOW){
        color_hold = 0;
        Serial.println("Red");
        digitalWrite(red_led,HIGH);
        digitalWrite(blue_led,LOW);
        delay(5000);
        digitalWrite(red_led,LOW);

    }
}
void line_recoery(){
    wihle(digitalRead(IR2)==LOW){ 
        move_backward(200,100);
        Brake(200);}
   
}

void Distance_sensor(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
}

void Rotate_to_opposite_dir(){
    //None
}
void sweep(){
    int limit_move = 0;
    while (distance>=max_bound){
        limit_move++;
    Rotate_right_inplace(100,0);
    if (limit_move > 70){break;}
    }
    Brake(300);
    while (distance>= reachable_bound){
        move_forward(100,0);
    }
    Brake(100);
    //color_detection
    //Servo object handling
    //line recvoery + move to opposite
    
    task = 1
}
void Rotate_right_until_match(){
    int limit_move = 0;
    while (digitalRead(IR2)== LOW | digitalRead(IR3)==HIGH ){
        limit_move++;
        Motor3->setSpeed(100);
        Motor3->run(FORWARD);
        Motor4->setSpeed(100);
        Motor4->run(BACKWARD);
        if (limit_move > 30){break;}

    }
    Brake(100);
}

void Rotate_left_until_match(){
    int limit_move = 0;
    while (digitalRead(IR2) == LOW | digitalRead(IR1)==HIGH ){
        limit_move++;
        Motor3->setSpeed(100);
        Motor3->run(BACKWARD);
        Motor4->setSpeed(100);
        Motor4->run(FORWARD);
        if (limit_move > 30){break;}
    }
    Brake(100);
}

void color_path(){
    if (color_hold==1){
        Rotate_left_inplace(200,2000);
        Brake(100);
    };
    if (color_hold==0){
        Rotate_right_inplace(200,2000);
        Brake(100);
    };
}

void loop() 

{ while(task == 0){
    Brake(50);
    Distance_sensor();

   if(digitalRead(IR1) == LOW  && digitalRead(IR2)==HIGH && digitalRead(IR3)==LOW) //IR will not glow on black line
  { 
    move_forward(200,450);
    
  }

  else if(digitalRead(IR1)== HIGH && digitalRead(IR2)== LOW && digitalRead(IR3)==LOW)  //IR not on black line
  {
    //Move both the Motors
   Rotate_left_until_match();
  }
   else if(digitalRead(IR1)== HIGH && digitalRead(IR2)== HIGH && digitalRead(IR3)==LOW)  //IR not on black line
  {
    //Move both the Motors
    Serial.println("Deviated left");
   Rotate_left_until_match();
   
  }
  else if(digitalRead(IR1) == LOW  && digitalRead(IR2)==HIGH && digitalRead(IR3)==HIGH)
  {
    //Tilt robot towards left by stopping the left wheel and moving the right one
    Serial.println("Deviated right");
    Rotate_right_until_match();
    
  }

  else if(digitalRead(IR1) == LOW  && digitalRead(IR2)==LOW && digitalRead(IR3)==HIGH)
  {
    //Tilt robot towards left by stopping the left wheel and moving the right one
    Rotate_right_until_match();
  }

  else if(digitalRead(IR1) == LOW && digitalRead(IR2) == LOW  && digitalRead(IR3) == LOW)
  { Rotate_right_until_match();
    line_recovery();
  }
  else if(digitalRead(IR1) == HIGH && digitalRead(IR2) == HIGH  && digitalRead(IR3) == HIGH){
      Brake(100);
      if (correct_distance){
      //perform specific task
      //sweep()
      }
  }
  }
  while (task==1){
    Brake(50);
    Distance_sensor();

   if(digitalRead(IR1) == LOW  && digitalRead(IR2)==HIGH && digitalRead(IR3)==LOW) //IR will not glow on black line
  { 
    move_forward(200,450);
    
  }

  else if(digitalRead(IR1)== HIGH && digitalRead(IR2)== LOW && digitalRead(IR3)==LOW)  //IR not on black line
  {
    //Move both the Motors
   Rotate_right_until_match();
  }

  else if(digitalRead(IR1) == LOW  && digitalRead(IR2)==LOW && digitalRead(IR3)==HIGH)
  {
    //Tilt robot towards left by stopping the left wheel and moving the right one
    Rotate_left_until_match();
  }

  else if(digitalRead(IR1) == LOW && digitalRead(IR2) == LOW  && digitalRead(IR3) == LOW)
  { Rotate_right_until_match();
    line_recovery();
  }
  else if(digitalRead(IR1) == HIGH && digitalRead(IR2) == HIGH  && digitalRead(IR3) == HIGH){
      Brake(100);
      if (correct_distance){
      //perform specific task
      
      }
  }
    
  }
}
 