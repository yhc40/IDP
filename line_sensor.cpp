
int IR1=8;      
int IR2=9;
int IR3 = 10;   
double threshold = 0.5;
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
{
  // put your setup code here, to run once:

  pinMode(IR1,INPUT);
  pinMode(IR2,INPUT);
  pinMode(IR3,INPUT);
}
void loop() 

{ while(going_to_collection_area){
    Brake(50);

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

 