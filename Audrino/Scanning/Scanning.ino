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


#define DIR_PIN 5
#define STEP_PIN 3

#define HOME_PIN 7
#define END_PIN 8

#define LASER_PIN 9

#define SCANBUTTON_PIN 10

#define BUTTON_LED_PIN 11


void setup() { 
  Serial.begin (9600); //com port communication
  pinMode(DIR_PIN, OUTPUT); 
  pinMode(STEP_PIN, OUTPUT);
  
  pinMode(HOME_PIN,INPUT);
  pinMode(END_PIN,INPUT); 
  
  pinMode(LASER_PIN,OUTPUT);
  
  pinMode(SCANBUTTON_PIN,INPUT);
  
  pinMode(BUTTON_LED_PIN,OUTPUT);
} 


void loop(){ 
  //findHome();
  if(Serial.available()){
    char input = (char)Serial.read();
    if(input == 's'){
      scanFoward();
    }
    if(input == 'h'){
      findHome();    
    }
    if(input == 'o'){
      ledOn();
    }
    if(input == 'e'){
      error();    
    } 
  }
  
  if(digitalRead(SCANBUTTON_PIN)==HIGH){
    Serial.write("B");//Button pressed
  }
}



void scanFoward(){
	float rate = 1; // mm/min
	float mmPs = rate*60;
	float revPmm = 1;
	float revPs = revPmm*mmPs;
	float distance = 200;//mm
	float distInRev = distance*revPmm;
	float time = distInRev/revPs;
	float stepsPRev = 200;
	float distInSteps = stepsPRev*distInRev;
	

        laserOn(); 
        ledOff();
	rotate(DIR_PIN,STEP_PIN,distInSteps);
        laserOff();
        ledOn();
}

void findHome(){
  bool stop = false;
  float steps = 10;
  while(digitalRead(HOME_PIN) == HIGH && !stop){
    rotate(DIR_PIN,STEP_PIN,steps);
  }
}
  

//// Laser
void laserOn(){
  digitalWrite(LASER_PIN,HIGH);
}

void laserOff(){
  digitalWrite(LASER_PIN,LOW);
}


//// LED CONTROLLS

void ledOn(){
  digitalWrite(BUTTON_LED_PIN, HIGH);
}

void ledOff(){
  digitalWrite(BUTTON_LED_PIN, LOW);
}


void error(){
  int brightness = 0;    // how bright the LED is
  int fadeAmount = 5*3;
  int led =  BUTTON_LED_PIN;
  
  while(true){// THIS CLAUSES THE LED TO SLOWLY BLINK FOREVER
    analogWrite(led, brightness);    
    // change the brightness for next time through the loop:
    brightness = brightness + fadeAmount;
    // reverse the direction of the fading at the ends of the fade: 
    if (brightness == 0 || brightness == 255) {
      fadeAmount = -fadeAmount ; 
    }     
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);   
  }
}



/////STEPPER CONTROLLER

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
