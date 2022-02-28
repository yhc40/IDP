
int IR1=8;      
int IR2=9;
int IR3 = 10;   
double threshold = 0.5;
int trigPin = 1;
int echoPin = 2; 
double max_bound = 30;
double reachable_bound = 5;
int task = 0;
// 0 is going to collection rea, 1 is delivering package, 2 is going back to initial area

long duration; // variable for the duration of sound wave travel
int distance;

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
void sweep(){
    int limit_move = 0;
    while (distance>=max_bound){
        limit_move++;
    Rotate_right_inplace(100,0);
    if (limit_move > 200){break;}
    }
    Brake(100);
    while (distance>= reachable_bound){
        move_forward(100,0);
    }
    Brake(100);
    //color_detection
    //Servo object handling
    //generate new task
    task = 1
}
void Rotate_right_until_match(){
    int limit_move = 0;
    while (analogRead(IR2)< threshold ){
        limit_move++;
        Motor3->setSpeed(100);
        Motor3->run(BACKWARD);
        Motor4->setSpeed(100);
        Motor4->run(BACKWARD);
        if (limit_move > 50){break;}

    }
    Brake(100);
}
void Rotate_left_until_match(){
    int limit_move = 0;
    while (analogRead(IR2)< threshold ){
        limit_move++;
        Motor3->setSpeed(100);
        Motor3->run(FORWARD);
        Motor4->setSpeed(100);
        Motor4->run(FORWARD);
        if (limit_move > 50){break;}
    }
    Brake(100);
}
void setup() 
{ pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); 
  // put your setup code here, to run once:

  pinMode(IR1,INPUT);
  pinMode(IR2,INPUT);
  pinMode(IR3,INPUT);
}
void loop() 

{ while(task == 0){
    Brake(50);
    Distance_sensor();

   if(analogRead(IR1)< threshold && analogRead(IR2)>=threshold && analogRead(IR3)<threshold) //IR will not glow on black line
  { 
    move_forward(150,100);
    
  }

  else if(analogRead(IR1)>= threshold && analogRead(IR2)<threshold && analogRead(IR3)<threshold)  //IR not on black line
  {
    //Move both the Motors
   Rotate_right_until_match();
  }

  else if(analogRead(IR1)< threshold && analogRead(IR2)<threshold && analogRead(IR3)>=threshold)
  {
    //Tilt robot towards left by stopping the left wheel and moving the right one
    Rotate_left_until_match();
  }

  else if(analogRead(IR1)< threshold && analogRead(IR2)<threshold && analogRead(IR3)<threshold)
  {
    line_recovery()/sweep();
  }
  else if(junction_detect){
      Brake(100);
      if (correct_distance){
      //perform specific task
      //sweep()
      }
  }
  while (task==1){
      //normal line following 
  }

 