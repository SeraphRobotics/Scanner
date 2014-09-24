  //////////////////////////////////////////////////////////////////
//©2014 Seraph Robotics, all rights reserved


/////////////////////////////////////////////////////////////////


#define DIR_PIN 5
#define STEP_PIN 3
#define ENABLE_PIN 4

#define HOME_PIN 7
#define END_PIN 8



#define SCANBUTTON_PIN 9

#define BUTTON_LED_PIN 10

#define LASER_PIN 11

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
    if(input == 'l'){
         laserOn();
    } 
    if(input == 'k'){
         laserOff();
    } 
  }
  
  if( !PRESSED && digitalRead(SCANBUTTON_PIN)==LOW){
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
	//float rate = 300; // mm/min
	float mmPs = 5;//rate/60;
	float revPmm = .0303*0.88*.98; //33mm circumrence
	float revPs = revPmm*mmPs;
	float distance = 335;//mm
	float distInRev = distance*revPmm;
	float time = distInRev/revPs;

	float distInDeg = distInRev*360.0; 
	

        bool end_hit = false;
        float deg = 0;
        float deg_increment=1;
        float rot_speed = revPs; // rev/second

        laserOn(); 
        ledOff();
        digitalWrite(DIR_PIN,HIGH);
        
        
        
        digitalWrite(ENABLE_PIN, LOW);
        while(!end_hit && (deg<distInDeg) ){
          deg= deg+deg_increment;
          rotateDeg(deg_increment,rot_speed);
          if( (digitalRead(END_PIN)==HIGH) ){ //||(digitalRead(HOME_PIN)==HIGH)
            end_hit=true;
            Serial.write("E");
            laserOff();
         } 
        }
        delay(1);
        //Serial.write("RETURN");
        //Serial.print(deg);  
        laserOff();
        findHome();
        digitalWrite(ENABLE_PIN, HIGH);
        
        ledOn();
        if (end_hit){
          error();
        }
        Serial.write("D");
}

void findHome(){
  bool end_hit = false;
  float deg_increment = 10;
  float rot_speed = 1.0;

  if(digitalRead(HOME_PIN)==HIGH){
        end_hit=true;
  }


  digitalWrite(ENABLE_PIN, LOW);
  while(!end_hit){
      rotateDeg(-deg_increment,rot_speed);
      if(digitalRead(HOME_PIN)==HIGH){
        end_hit=true;
     } 
  }
  digitalWrite(ENABLE_PIN, HIGH);
  
  
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
  int fadeAmount = 5*2;
  int led =  BUTTON_LED_PIN;
  
  int n=0;
  while(n<25){// THIS CLAUSES THE LED TO SLOWLY BLINK FOREVER
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

void rotateDeg(float deg, float rot_speed){ 
  //rotate a specific number of degrees (negitive for reverse movement)
  //rotational speed is in revolutions per second
  int dir = (deg > 0)? HIGH:LOW;
  digitalWrite(DIR_PIN,dir); 

  int microsteps = 32;

  int steps = microsteps*abs(deg)*(1/1.8);//(1/0.225);
  float usDelay = 1000000.0/(microsteps*200.0*rot_speed);

  for(int i=0; i < steps; i++){ 
    digitalWrite(STEP_PIN, HIGH); 
    delayMicroseconds(10); 

    digitalWrite(STEP_PIN, LOW); 
    delayMicroseconds(usDelay); 
  } 
}
