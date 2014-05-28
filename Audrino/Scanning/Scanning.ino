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
#define ENABLE_PIN 12

#define HOME_PIN 7
#define END_PIN 8

#define LASER_PIN 9

#define SCANBUTTON_PIN 10

#define BUTTON_LED_PIN 11

bool PRESSED = false;

void setup() { 
  Serial.begin (9600); //com port communication
  pinMode(DIR_PIN, OUTPUT); 
  pinMode(STEP_PIN, OUTPUT);
  
  pinMode(HOME_PIN,INPUT);
  pinMode(END_PIN,INPUT); 
  
  pinMode(LASER_PIN,OUTPUT);
  
  pinMode(SCANBUTTON_PIN,INPUT);
  
  pinMode(BUTTON_LED_PIN,OUTPUT);
  pinMode(ENABLE_PIN,OUTPUT);
  
  digitalWrite(ENABLE_PIN, HIGH);// oin is !ENabl
  fast_Blink();
} 


void loop(){ 
  if(Serial.available()){
    char input = (char)Serial.read();
    if(input == 's'){
      scanFoward();
      PRESSED=false;
    }
    if(input == 'h'){
      ledOff();
      findHome();
      ledOn();
      PRESSED=false;    
    }
    if(input == 'o'){
      ledOn();
      PRESSED=false;
    }
    if(input == 'f'){
      ledOff();
      PRESSED=false;
    }
    if(input == 'e'){
      error();    
    } 
  }
  
  if( !PRESSED && digitalRead(SCANBUTTON_PIN)==HIGH){
    Serial.write("B");//Button pressed
    PRESSED=true;
  }
  if(digitalRead(HOME_PIN)==HIGH){
    //Serial.write("H");//Button pressed
  }
  if(digitalRead(END_PIN)==HIGH){
    //Serial.write("E");//Button pressed
  }
  
  
  
}



void scanFoward(){
	float rate = 1; // mm/min
	float mmPs = rate*60;
	float revPmm = .01;
	float revPs = revPmm*mmPs;
	float distance = 200;//mm
	float distInRev = distance*revPmm;
	float time = distInRev/revPs;

	float distInDeg = distInRev*360.0; 
	

        bool end_hit = false;
        float deg = 0;
        float deg_increment=15;
        float rot_speed = 0.5;

        laserOn(); 
        ledOff();
        digitalWrite(DIR_PIN,HIGH);
        
        
        
        digitalWrite(ENABLE_PIN, LOW);
        while(!end_hit && (deg<distInDeg) ){
          deg= deg+deg_increment;
          rotateDeg(deg_increment,rot_speed);
          if(digitalRead(END_PIN)==HIGH){
            end_hit=true;
            laserOff();
         } 
        }
        
        
        rotateDeg(-distInDeg,rot_speed);
        digitalWrite(ENABLE_PIN, HIGH);
        laserOff();
        ledOn();
        if (end_hit){
          error();
        }
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


void fast_Blink(){
  int brightness = 0;    // how bright the LED is
  int fadeAmount = 5*5;
  int led =  BUTTON_LED_PIN;
  
  int n=0;
  while(n<4){// THIS CLAUSES THE LED TO SLOWLY BLINK FOREVER
    analogWrite(led, brightness);    
    // change the brightness for next time through the loop:
    brightness = brightness + fadeAmount;
    // reverse the direction of the fading at the ends of the fade: 
    if (brightness == 0 || brightness == 255) {
      fadeAmount = -fadeAmount ; 
      
    }     
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);
    n=n+1;    
  }
  ledOff();
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
