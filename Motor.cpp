void setup() {

  Serial.begin(9600);

  drive(0,50);

}

 

static int EnA = 11; // PWM

static int In1 = 10;

static int In2 = 9;

static int In3 = 7;

static int In4 = 8;

static int EnB = 6; // PWM

void drive(int direction, int speed){ //direction = 0: Straight, direction = 1 or 2: Left or Right

  if(direction == 1){

    analogWrite(EnA, speed);

    digitalWrite(In1, HIGH);

    digitalWrite(In2, LOW);

//    analogWrite(EnB, speed);

    digitalWrite(In3, LOW);

    digitalWrite(In4, LOW);

  }else if( direction == 2){ 

    analogWrite(EnB, speed);

    digitalWrite(In3, HIGH);

    digitalWrite(In4, LOW);

//    analogWrite(EnA, speed);

    digitalWrite(In1, LOW);

    digitalWrite(In2, LOW);

  }else{ 

    analogWrite(EnA, speed);

    digitalWrite(In1, HIGH);

    digitalWrite(In2, LOW);

   

    analogWrite(EnB, speed);

    digitalWrite(In3, HIGH);

    digitalWrite(In4, LOW);

  }

}

 

void stop_motors(){

  analogWrite(EnA, 0);

  digitalWrite(In1, LOW);

  digitalWrite(In2, LOW);

 

  analogWrite(EnB, 0);

  digitalWrite(In3, LOW);

  digitalWrite(In4, LOW);

}

 

void motorSetup() {

  // put your setup code here, to run once:

  pinMode(In1, OUTPUT);

  pinMode(In2, OUTPUT);

  pinMode(In3, OUTPUT);

  pinMode(In4, OUTPUT);

  pinMode(EnA, OUTPUT);

  pinMode(EnB, OUTPUT);

 

  digitalWrite(In1, LOW);

  digitalWrite(In2, LOW);

 

  digitalWrite(In3, LOW);

  digitalWrite(In4, LOW);

 

}

 

void loop() {

  // put your main code here, to run repeatedly:

 

}

 

 