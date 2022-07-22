#define A 20     
#define B 21    
#include <MsTimer2.h>     //include library for timer interrupt

int pos = 0;     
int old_pos=0;
int ppu=0;
String rotateDirec="";


void setup() {
  pinMode(A,INPUT);     
  pinMode(B,INPUT);     
  attachInterrupt(3,BladeCountA,CHANGE);     //interuppt pin number of Arduino Mega 2560 pin number 21 is INT.2
  attachInterrupt(2,BladeCountB,CHANGE);     //interuppt pin number of Arduino Mega 2560 pin number 21 is INT.3
  Serial.begin(4800);     

  MsTimer2::set(50,timerISR);      //execute timerISR every 50ms after timer interrupt starts
  MsTimer2::start();     //activate timer
}



void BladeCountA() {
  if(digitalRead(A)==HIGH) {    
    if(digitalRead(B)==LOW) {    
    pos ++;   
    rotateDirec="cw";   
    }else{     
    pos --;    
    rotateDirec="ccw";    
    }
  }
  else{  
    if(digitalRead(B)==HIGH) {     
    pos ++;     
    rotateDirec="cw";    
    }else{     
    pos --;     
    rotateDirec="ccw";     
    }
  }
}



void BladeCountB() {
  if(digitalRead(B)==HIGH) {     
    if(digitalRead(A)==HIGH) {     
    pos ++;     
    rotateDirec="cw";     
    }else{     
    pos --;     
    rotateDirec="ccw";     
    }
  }
  else{     
    if(digitalRead(A)==LOW) {     
    pos ++;     
    rotateDirec="cw";     
    }else{     
    pos --;     
    rotateDirec="ccw";     
    }
  }
}

/************************************************************************************************************/

void timerISR() {
    ppu=(pos-old_pos);     //M method(how many pulses we receive in 1 second) which is advantageous at high speed
  old_pos=pos;
}

void loop() {
  Serial.print("deirection:");
  Serial.print(rotateDirec);    
  Serial.print("  ");
  Serial.print("position:");
  Serial.print(pos);     
  Serial.print("  ");
  Serial.print("ppu:");
  Serial.println(ppu);    
}
