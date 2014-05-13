//////////////////////////////////////////////////////////////////
//©2011 bildr
//Released under the MIT License - Please reuse change and share
//Using the easy stepper with your arduino
//use rotate and/or rotateDeg to controll stepper motor
//speed is any number from .01 -> 1 with 1 being fastest - 
//Slower Speed == Stronger movement


//rotate a specific number of microsteps (8 microsteps per step)
//a 200 step stepper would take 1600 micro steps for one full revolution
//rotate(1600, .5); 
//delay(1000); 
//rotate(-1600, .25); //reverse
//delay(1000); 

/////////////////////////////////////////////////////////////////


#define DIR_PIN 4
#define STEP_PIN 5


void setup() { 
  Serial.begin (9600); //com port communication
  pinMode(DIR_PIN, OUTPUT); 
  pinMode(STEP_PIN, OUTPUT); 
} 


void loop(){ 
  if(Serial.available()){
    char input = (char)Serial.read();
    if(input == 'w'){
      rotate(DIR_PIN,STEP_PIN,64);
    }
    if(input == 'a'){
      rotate(-DIR_PIN,STEP_PIN,64);
  } 
}



void scanFoward(){
	float rate = 1; // mm/min
	float mmPs = rate*60;
	float revPmm = 1;
	float revPs = revPmm*mmPs;
	float distance = 200;/mm
	float distInRev = distance*revPmm;
	float time = distInRev/revPs;
	float stepsPRev = 200;
	float distInSteps = stepPRev*distInRev;
	
	rotate(DIR_PIN,STEP_PIN,distanceInSteps);
}




void rotate(int const dir_p, int const stp_p, int steps){ 
  //rotate a specific number of microsteps (8 microsteps per step) - (negitive for reverse movement)
  //speed is any number from .01 -> 1 with 1 being fastest - Slower is stronger
  int dir = (steps > 0)? HIGH:LOW;
  steps = abs(steps);

  digitalWrite(dir_p,dir); 

  float usDelay = 100;

  for(int i=0; i < steps; i++){ 
    digitalWrite(stp_p, HIGH); 
    delayMicroseconds(usDelay); 

    digitalWrite(stp_p, LOW); 
    delayMicroseconds(usDelay); 
  } 
} 

void rotateDeg(float deg, float speed){ 
  //rotate a specific number of degrees (negitive for reverse movement)
  //speed is any number from .01 -> 1 with 1 being fastest - Slower is stronger
  int dir = (deg > 0)? HIGH:LOW;
  digitalWrite(DIR_PIN,dir); 

  int steps = abs(deg)*(1/0.225);
  float usDelay = 700;

  for(int i=0; i < steps; i++){ 
    digitalWrite(STEP_PIN, HIGH); 
    delayMicroseconds(usDelay); 

    digitalWrite(STEP_PIN, LOW); 
    delayMicroseconds(usDelay); 
  } 
}
