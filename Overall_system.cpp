// dependencies and libraries
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position

int trigPin = 11; //ultrasonic sensor pins on arduino
int echoPin = 10; 
long duration; // duration of sound wave travel
int distance; //distance measured by ultrasonic sensor 

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); //Motor set up on arduino 
Adafruit_DCMotor *Motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *Motor4 = AFMS.getMotor(4);

int IR1=4;     //pins of line sensors, IR1 is left sensor, IR2 is middle sensor, IR3 is right sensor.
int IR2=5;
int IR3 = 6;   

int color_sensor = 9 ; // pins of color_sensor, and their corresponding LED as signal 
int blue_led = 3 ;
int red_led = 8;
int color_hold = 0; // store the color of block detected, 0 is red and 1 is blue 

double max_bound = 50; // length of diagonal of collection area
double reachable_bound = 27; // fine-tune distance for arm grabbing

int load_delivered = 0; //store the number of blocks successfully delivered by robot
int load_carrying = 0; // tell us whether the robot is currently carrying a block, 0 = None, 1 = carrying
int task = 0; //task= 0 while load_delivered < 4 indicates navigating to collection area, task = 1 means delivering block

int moving_led = 7; //flashing led while moving, pins on arduino 

int white_squares_availability [2] = {0,0}; //array that  store whether the white squares are already occupied by block.
// 0 means unoccupied, 1 means occupied and we need to navigate to the remaining one. As there are only 2 squares per color, 
//array of size 2 is sufficient to tell us.

void setup() 
{ AFMS.begin();
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); 
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

//Open and close the servo arm to a specified angle and angular velocity
void open_arm(int angle, int angular_velocity, int max_angle){
    if(angle<max_angle){
         for (pos = angle; pos <= max_angle; pos += angular_velocity) {
    myservo.write(pos);            
    delay(15);                       
}}}


void close_arm(int angle, int angular_velocity){
  if(angle >=0){
    for (pos = angle; pos >= 30; pos -= angular_velocity) { 
    myservo.write(pos);              
    delay(15);
}}}


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
  // limit_move is to avoid stuck in infinity loop
    int limit_move = 0;
    // rotating and sweeping for the block in collection area, max_bound is the length of diagonal of the collection area.
    while (distance>=max_bound){
      Distance_sensor();
        limit_move++;
    Rotate_right_inplace(100,200);
    if (limit_move > 70){break;}
    }
    Brake(300);
    // The block is detected in that angle/direction, the robot approach the block slowly until the arm can reach it
    // reachable_bound is the maximum distance arm can grab it
    while (distance>= reachable_bound){
        Distance_sensor();
        move_forward(100,100);
    }
    Brake(100);
;}


 void color_detection(){
  delay(1000);
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
    }}


// s control speed, t control time toggle on that speed
void move_forward(double s,double t){
     Motor3->setSpeed(s);
     Motor4->setSpeed(s);
    Motor3->run(FORWARD);
    Motor4->run(FORWARD);
    digitalWrite(moving_led,HIGH);
    delay(t);
    digitalWrite(moving_led,LOW);
}


void move_backward(double s,double t){
    Motor3->setSpeed(s);
    Motor4->setSpeed(s);
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


// turn right/left while moving forward (not inplace), back-up function for tackling smooth curves if there are any
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


 //basic line_recovery, we assume the robot overshoot and  just simply move backward slowly to find the line 
 // simple but works better than expected, specifically tackle unbounded black area line lost
 void line_recovery(){
    while(digitalRead(IR2)==LOW & digitalRead(IR1)==LOW & digitalRead(IR3)==LOW){ 
        move_backward(200,100);
        Brake(100);}
}


// similar concept, just move forward instead of backward, search for line when we know we are bounded
void search_line(){
  while(digitalRead(IR2)==LOW & digitalRead(IR1)==LOW & digitalRead(IR3)==LOW){ 
        move_forward(200,100);
        Brake(100);}
  }


//use in line following, correct the deviated angle slowly until only middle sensors are aligned with lines
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
    Brake(50);}


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
    Brake(100);}


// decide how to navigate to specific area base on color detected
//the values of speed and toggle time are obtained by trial and error, 
//we don't use line following temporarily when this function is called to avoid sharp turns and inconvenience (edge cases)
void color_path(){
    if (color_hold==1){
        if (white_squares_availability[1]==0){
            move_forward(200,2400);
            white_squares_availability[1]=1;
        };
        else{
            move_backward(200,1600);
        };
        Rotate_left_inplace(200,2600);
        move_forward(200,2800);};

    if (color_hold==0){
        if (white_squares_availability[0]==0){
            move_forward(200,2700);
            white_squares_availability[0]=1;
        };
        else{
            move_backward(200,1600);
        };
    Rotate_right_inplace(200,1700);
    move_forward(200,2600);};
} 

void junction(){
    if (task==0){
        if (distance <=55 && load_carrying == 0){ //specific junction detected
      sweep();
      
      open_arm(30,3,120);
      
      move_forward(210,3450); //correction distance when approaching block 
      Brake(100);
      color_detection(); //detect color
      close_arm(120,3);
      delay(800); //finish grabbing
      
      Rotate_left_inplace(220,3200); //rotate 180 degrees to find way back
      load_carrying = 1; 
      task = 1;       //switch to deliver blocks
       Distance_sensor();}
   else{ //not junction we want, move forward as usual
    Brake(50);
    move_forward(230,300);
    Distance_sensor();
  }

    }
else if (task==1){
    if (distance <=105 && distance>=60){ // blue-red junction detected

      color_path(); //navigate base on color detected

      open_arm(30,3,120); //release the block 
      delay(1000);

      move_backward(200,2500); //correction distance when leaving the red/blue paths
      line_recovery(); //backtrack to main line 
      close_arm(120,3); //close arm here to avoid arm collide with block

      task = 0; //switch back to collecting blocks 
      load_carrying = 0; //not holding block anymore
      load_delivered ++;
      if (color_hold == 1 && load_delivered >=4){ //navigate back to starting area
        Rotate_right_inplace(200,1600);
        move_forward(200,3600);
        Brake(50);
        while(true){delay(10000);}
        }
       else if (color_hold == 0 && load_delivered >=4){ //navigate back to starting area
        Rotate_left_inplace(200,1600);
        Brake(50);
        move_forward(200,3600);
        Brake(50);
        while(true){ delay(10000);}
        }
       Distance_sensor();}
   else{ //move forward as it is not specific junction
    Brake(50);
    move_forward(230,300);
     Distance_sensor();
  }}}


void line_follow(){

//go straight
if(digitalRead(IR1) == LOW  && digitalRead(IR2)==HIGH && digitalRead(IR3)==LOW) {
   move_forward(200,450);
}

//deviated right
else if(digitalRead(IR1)== HIGH && (digitalRead(IR2)== LOW || digitalRead(IR2)== HIGH) && digitalRead(IR3)==LOW) {
   Rotate_left_until_match();
   }

//deviated left
else if(digitalRead(IR1) == LOW  && (digitalRead(IR2)== LOW || digitalRead(IR2)==HIGH)  && digitalRead(IR3)==HIGH) {    
   Rotate_right_until_match();
   }

//route recovery
else if(digitalRead(IR1) == LOW && digitalRead(IR2) == LOW  && digitalRead(IR3) == LOW){
    move_forward(220,1500);
    line_recovery();  
   }

//junction
else if(digitalRead(IR1) == HIGH && digitalRead(IR2) == HIGH  && digitalRead(IR3) == HIGH){
    Distance_sensor();
   junction();
   }
}


void loop() 
{ 
Brake(5000); //time for unplugging cable
search_line(); //search line when robot start inside the area, only use once in beginning (bounded searching)

//a self define loop that will perform all the task required
while(true){
Distance_sensor();
line_follow();
};
}