

double distance;
double detect_bounds;
double reachable_bounds;
int distance_sensor = 1;
int color_sensor = 2;
bool load_carried = false;
int blocks_delivered = 0;
bool color_detected = false;
int left_sensor = 3;
int middle_sensor = 4;
int right_sensor = 5;
int LFSensor[3]={0, 0, 0};
int current_junction_number = 0; //addition or subtraction base on current task, global record 
int counter_for_break_infinity = 0;
double K_p;



void initialize(){
    pinMode(distance_sensor,INPUT);
    pinMode(color_sensor,INPUT);}

void line_following(){

}

void junction_detection(){
    if (LFSensor[0]==1 & LFSensor[1]==1 & LFSensor[2]==1){//junction detected
    if (current_task()==)}
    
}
void object_handling()
{
    //grabbing or shovelling , set load_carried to true; 
}
void sweep_block_detection(){//rotate in place;
//return current distance of block and orientation
Distance_of_block();
if (distance <= detect_bounds){
    while(reachable_bounds < distance){//steer in that direction smaller than detect_bounds
    }
    while (color_detected == false){color_detection();}
    object_handling();}
if (distance >= detect_bounds){
    //task is go back
}
    
}

void color_detection(){
    analogRead(color_sensor);
    //color  threshold here 
    //if color is detected, light corresponding led with timedelay(), set color_detected to true
    
}

void current_task(){
    if (load_carried){//current task is delivering ; navigate to red/blue zone 
    }
    if (load_carried == false){//navigating to collection area;
    }


}
void Distance_of_block()
{distance = analogRead(distance_sensor);};


 

                          