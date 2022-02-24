static int EnA = 11; // PWM
static int In1 = 10;
static int In2 = 9;
static int In3 = 7;
static int In4 = 8;
static int EnB = 6; // PWM
//static int LDRB = 12;
//static int LDRR = 0;
//static int LDRL = 1;
//static int LDRF = 3;
//int light_val = 0;
void setup() {
 Serial.begin(9600);
 initialize();
}
void initialize(){
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
  pinMode(EnA, OUTPUT);
  pinMode(EnB, OUTPUT);
//  pinMode(LDRB, INPUT);
//  pinMode(LDRF, INPUT);
//  pinMode(LDRR, INPUT);
//  pinMode(LDRL, INPUT);
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);
}
//void highest_light (){
//  int light_val_L = analogRead(LDRL);
//  int light_val_R = analogRead(LDRR);
//  //int light_val_B = analogRead(LDRB);
//  int light_val_F = analogRead(LDRF);
//  if ((light_val_L == light_val_R)&&(light_val_R == light_val_F)) {
//    stop();
//  }
//  
//  if ((light_val_L > light_val_R)&&(light_val_L > light_val_F)) {
//    left(255);
//  }
//  else {
//    if ((light_val_L > light_val_F)) {
//      right(255);
//    }
//    else{
//        forward(255);
//      }
//       
//  } 
//}

void stop(){
  analogWrite(EnA, 0);
  analogWrite(EnB, 0);
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);
}

void left(int speed){
  analogWrite(EnA, speed);
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, HIGH);
}
void right(int speed){
  analogWrite(EnB, speed);
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
  digitalWrite(In1, HIGH);
  digitalWrite(In2, HIGH);
}
void backward(int speed){
  analogWrite(EnA, speed);
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
    
  analogWrite(EnB, speed);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
}
void forward(int speed){
  analogWrite(EnA, speed);
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
    
  analogWrite(EnB, speed);
  digitalWrite(In3, LOW);                                                                                                                     
  digitalWrite(In4, HIGH);
}

void move(String direction, int speed){
  if (direction == "Left"){
    left(speed);
  }
  else if (direction == "Right"){
    right(speed);
  }
  else if (direction == "Forward"){
    forward(speed);
  }
  else{
    backward(speed);
  }
}

void loop() {
  move("Forward",255);
  delay(000);
}