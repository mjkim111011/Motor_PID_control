#define A 20     //encoder channel A
#define B 21     //encoder channel B

int pos = 0;     //encoder initial position
String rotateDirec="";


void setup() {
  pinMode(A,INPUT);     //channel A value
  pinMode(B,INPUT);     //channel B value
  attachInterrupt(3,BladeCountA,CHANGE);     
  attachInterrupt(2,BladeCountB,CHANGE);     
  Serial.begin(4800);     //start serial communication

}


void BladeCountA() {
  
  if(digitalRead(A)==HIGH) {     //when channel A is Rising,
    if(digitalRead(B)==LOW) {     //if channel B Falls,
    pos ++;    //motor rotates in clockwise direction
    rotateDirec="clockwise";    //print "clockwise"
    
    }else{     //if channel B Rises,
    pos --;    //motor rotates in counter clockwise direction
    rotateDirec="counterclockwise";    //print "counterclockwise"
    }
  }
  else{  //when channel A is Falling,
    if(digitalRead(B)==HIGH) {     //if channel B Rises
    pos ++;     //motor rotates in clockwise direction
    rotateDirec="clockwise";     //print "clockwise"
    
    }else{     //if channel B Falls,
    pos --;     //motor rotates in counter clockwise direction
    rotateDirec="counterclockwise";     //print "counterclockwise"
    }
  }
}



void BladeCountB() {
  if(digitalRead(B)==HIGH) {     //when channel B is Rising, 
    if(digitalRead(A)==HIGH) {     //if channel A Rises,
    pos ++;     //motor rotates in clockwise direction
    rotateDirec="clockwise";     //print "clockwise"
    
    }else{     //if channel B Falls,
    pos --;     //motor rotates in counter clockwise direction
    rotateDirec="counterclockwise";     //print "counterclockwise"
    }
  }
  else{     //when channel B is Falling,
    if(digitalRead(A)==LOW) {     //if channel A Falls,
    pos ++;     //motor rotates in clockwise direction
    rotateDirec="clockwise";     //print "clockwise"
    
    }else{     //if channel A Rises,
    pos --;     //motor rotates in counter clockwise direction
    rotateDirec="counterclockwise";     //print "counterclockwise"
    }
  }
}



void loop() {
  Serial.print("direction: ");
  Serial.print(rotateDirec);     //print rotating direction
  Serial.print("\t");
  Serial.print("position");
  Serial.println(pos);     //print position
}
