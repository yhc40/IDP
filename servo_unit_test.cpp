
#include <Servo.h>
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
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
void object_detection(){

}
void loop() {
  open_arm(15,3,90);
  delay(700);
  close_arm(90,3);
  delay(500);
  
}