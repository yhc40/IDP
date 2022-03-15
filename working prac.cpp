#include <Wire.h>

#include <Adafruit_MotorShield.h>

#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <Servo.h>
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

int trigPin = 11;
int echoPin = 10; 
int moving_led = 7;
int load_carrying = 0;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *Motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *Motor4 = AFMS.getMotor(4);

int IR1=4;      
int IR2=5;
int IR3 = 6;   

int color_sensor = 9 ;
int blue_led = 3 ;
int red_led = 8;

double max_bound = 50;
double reachable_bound = 30;

int task = 0;
int load_delivered = 0;
int color_hold = 0;


//0 is blue, 1 is red etc

// 0 is going to collection rea, 1 is delivering package, 2 is going back to initial area

long duration; // variable for the duration of sound wave travel
int distance;



void setup() 
{ AFMS.begin();
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); 
  pinMode(moving_led,OUTPUT);
  pinMode(IR1,INPUT);
  pinMode(IR2,INPUT);
  pinMode(IR3,INPUT);
  pinMode(color_sensor,INPUT);
  pinMode(blue_led,OUTPUT);
  pinMode(red_led,OUTPUT);
  
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
void Distance_sensor(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  Serial.println(distance)
;}
void sweep(){
  Distance_sensor();
    int limit_move = 0;
    while (distance>=max_bound){
      Distance_sensor();
      
        limit_move++;
    Rotate_right_inplace(100,200);
    if (limit_move > 70){break;}
    }
    Brake(300);
    while (distance>= reachable_bound){
      Distance_sensor();
        move_forward(100,100);
    }
    Brake(100);
    
//    color_detection();
    //Servo object handling
    //line recvoery + move to opposite
    
//    task = 1
;}
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
void close_arm(int angle, int angular_velocity){
  if(angle >=0){
    for (pos = angle; pos >= 30; pos -= angular_velocity) { 
    myservo.write(pos);              
    delay(15);                       
  }
    }
  }
void move_forward(double s,double t){
     Motor3->setSpeed(s);
     Motor4->setSpeed(s+10);
    Motor3->run(FORWARD);
    
    Motor4->run(FORWARD);
    digitalWrite(moving_led,HIGH);
   
    delay(t);
    digitalWrite(moving_led,LOW);
}

void move_backward(double s,double t){
    Motor3->setSpeed(s);
    
    Motor4->setSpeed(s-8);
    Motor3->run(BACKWARD);
    Motor4->run(BACKWARD);
    digitalWrite(moving_led,HIGH);
   
    delay(t);
    digitalWrite(moving_led,LOW);
  
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
    digitalWrite(moving_led,HIGH);
   digitalWrite(moving_led,LOW);
    delay(t);

}

 void Rotate_right_inplace(double s, double t){
    Motor3->setSpeed(s);
    Motor4->setSpeed(s);
    Motor3->run(FORWARD);
    
    Motor4->run(BACKWARD);
    digitalWrite(moving_led,HIGH);
   digitalWrite(moving_led,LOW);
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
 void line_recovery(){
    while(digitalRead(IR2)==LOW & digitalRead(IR1)==LOW & digitalRead(IR3)==LOW){ 
        move_backward(200,100);
        Brake(100);}
   
}


void Rotate_right_until_match(){


    int limit_move = 0;
    Distance_sensor();
    while (digitalRead(IR2)== LOW | digitalRead(IR3)==HIGH ){
     
        limit_move++;
        Motor3->setSpeed(200);
        Motor3->run(FORWARD);
        Motor4->setSpeed(200);
        Motor4->run(BACKWARD);
        if (limit_move > 30)
        {break;}
        

    }
    
    Brake(50);
}
void debug(){
  if(digitalRead(IR2)==HIGH)
  {Serial.println("High");}
  else if (digitalRead(IR2)==LOW){
    Serial.println("Low");}
    }
 void debug_l(){
  if(digitalRead(IR1)==HIGH)
  {Serial.println("Left is High");}
  else if (digitalRead(IR1)==LOW){
    Serial.println("Left is Low");}
    }
    void debug_r(){
  if(digitalRead(IR3)==HIGH)
  {Serial.println("Right is High");}
  else if (digitalRead(IR3)==LOW){
    Serial.println("Right Low");}
    }
void Rotate_left_until_match(){
    int limit_move = 0;
    while (digitalRead(IR2) == LOW | digitalRead(IR1)==HIGH ){
        limit_move++;
        Motor3->setSpeed(150);
        Motor3->run(BACKWARD);
        Motor4->setSpeed(150);
        Motor4->run(FORWARD);
        if (limit_move > 50){break;}
    }
    Brake(100);
}
void color_path(){
    if (color_hold==1){
      move_forward(200,1200);
      Brake(100);
        Rotate_left_inplace(200,1900);
        Brake(100);
        move_forward(200,2000);
        Brake(100);
    };
    if (color_hold==0){
      move_forward(200,1200);
      Brake(100);
        Rotate_right_inplace(200,1900);
        Brake(100);
        move_forward(200,2000);
        Brake(100);
    };
}
void loop() 

{ 

Brake(5000);
open_arm(10,3,110);
      delay(800);
      close_arm(110,3);
      
      delay(800);

  
  while(task == 0){
    Distance_sensor();
  
   digitalWrite(moving_led,HIGH);
    
    
    Serial.println("loop");
    Brake(60);
    digitalWrite(moving_led,LOW);

   if(digitalRead(IR1) == LOW  && digitalRead(IR2)==HIGH && digitalRead(IR3)==LOW) //IR will not glow on black line
  { 
    move_forward(200,100);
    
   
    
  }
  else if(digitalRead(IR1)== HIGH && digitalRead(IR2)== HIGH && digitalRead(IR3)==LOW)  //IR not on black line
  {
    //Move both the Motors
  
   Rotate_left_until_match();
   
  }
  else if(digitalRead(IR1) == LOW  && digitalRead(IR2)==HIGH && digitalRead(IR3)==HIGH)
  {
    //Tilt robot towards left by stopping the left wheel and moving the right one
    
    Rotate_right_until_match();
    
  }

  else if(digitalRead(IR1)== HIGH && digitalRead(IR2)== LOW && digitalRead(IR3)==LOW)  //IR not on black line
  {
    //Move both the Motors
    
   Rotate_left_until_match();
   
  }

  else if(digitalRead(IR1) == LOW  && digitalRead(IR2)==LOW && digitalRead(IR3)==HIGH)
  {
    //Tilt robot towards left by stopping the left wheel and moving the right one
    
    Rotate_right_until_match();
    
  }

  else if(digitalRead(IR1) == LOW && digitalRead(IR2) == LOW  && digitalRead(IR3) == LOW)
  { 
    move_forward(230,1500);

  line_recovery();
 
  
    
}
else if(digitalRead(IR1) == HIGH && digitalRead(IR2) == HIGH  && digitalRead(IR3) == HIGH){
//  move_forward(220,50);
  Distance_sensor();
//  delay(3500);
//  move_forward(200,2500);
//     Brake(300);
//      color_path();
//      move_forward(220,2100);
//      Brake(500);
  if (distance <=55 && load_carrying == 0){
      sweep();
      
      open_arm(30,3,120);
      delay(600);
      move_forward(200,3150);
      Brake(100);
      color_detection();
      close_arm(120,3);
      
      delay(800);
      
      Rotate_left_inplace(220,3500);
      load_carrying = 1;
      task = 1;
       Distance_sensor();}
   else{
    Brake(50);
    move_forward(230,300);
     Distance_sensor();
  }
      
      
}}
while(task==1){
    Distance_sensor();
  
   digitalWrite(moving_led,HIGH);
    
    
    Serial.println("loop");
    Brake(60);
    digitalWrite(moving_led,LOW);

   if(digitalRead(IR1) == LOW  && digitalRead(IR2)==HIGH && digitalRead(IR3)==LOW) //IR will not glow on black line
  { 
    move_forward(200,100);
    
   
    
  }
  else if(digitalRead(IR1)== HIGH && digitalRead(IR2)== HIGH && digitalRead(IR3)==LOW)  //IR not on black line
  {
    //Move both the Motors
  
   Rotate_left_until_match();
   
  }
  else if(digitalRead(IR1) == LOW  && digitalRead(IR2)==HIGH && digitalRead(IR3)==HIGH)
  {
    //Tilt robot towards left by stopping the left wheel and moving the right one
    
    Rotate_right_until_match();
    
  }

  else if(digitalRead(IR1)== HIGH && digitalRead(IR2)== LOW && digitalRead(IR3)==LOW)  //IR not on black line
  {
    //Move both the Motors
    
   Rotate_left_until_match();
   
  }

  else if(digitalRead(IR1) == LOW  && digitalRead(IR2)==LOW && digitalRead(IR3)==HIGH)
  {
    //Tilt robot towards left by stopping the left wheel and moving the right one
    
    Rotate_right_until_match();
    
  }

  else if(digitalRead(IR1) == LOW && digitalRead(IR2) == LOW  && digitalRead(IR3) == LOW)
  { 
    move_forward(230,1500);

  line_recovery();
 
  
    
}
else if(digitalRead(IR1) == HIGH && digitalRead(IR2) == HIGH  && digitalRead(IR3) == HIGH){
//  move_forward(220,50);
  Distance_sensor();
//  delay(3500);
//  move_forward(200,2500);
//     Brake(300);
//      color_path();
//      move_forward(220,2100);
//      Brake(500);
  if (distance <=105 && load_carrying == 1 && distance>=80){
      color_path();
       Distance_sensor();}
   else{
    Brake(50);
    move_forward(230,300);
     Distance_sensor();
  }
      

}
}


//calibrated 
void color_path(){
    if (color_hold==1){
      move_forward(200,2400);
      Brake(100);
        Rotate_left_inplace(200,2600);
        Brake(100);
        move_forward(200,2800);
        Brake(100);
    };
    if (color_hold==0){
      move_forward(200,2100);
      Brake(100);
        Rotate_right_inplace(200,1700);
        Brake(100);
        move_forward(200,2100);
        Brake(100);
    };
} 