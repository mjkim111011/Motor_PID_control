#define A 20     //A상
#define B 21     //B상

int pos = 0;     //엔코더 위치 초기값
String rotateDirec="";


void setup() {
  pinMode(A,INPUT);     //A상 값
  pinMode(B,INPUT);     //B상 값
  attachInterrupt(3,BladeCountA,CHANGE);     
  attachInterrupt(2,BladeCountB,CHANGE);     
  Serial.begin(4800);     //시리얼 통신 시작

}
/*******************************************************************************************************/

void BladeCountA() {
  if(digitalRead(A)==HIGH) {     //A가 Rising 할 때
    if(digitalRead(B)==LOW) {     //B가 Falling 한다면
    pos ++;    //시계 방향으로 돌고
    rotateDirec="clockwise";    //"clockwise" 출력
    }else{     //B가 Rising한다면
    pos --;    //반시계 방향으로 돌고
    rotateDirec="counterclockwise";    //"counterclockwise" 출력
    }
  }
  else{  //A가 Falling할 때
    if(digitalRead(B)==HIGH) {     //B가 Rising 한다면
    pos ++;     //시계 방향으로 돌고
    rotateDirec="clockwise";     //"clockwise" 출력
    }else{     //B가 Falling한다면
    pos --;     //반시계 방향으로 돌고
    rotateDirec="counterclockwise";     //"counterclockwise" 출력
    }
  }
}

/**************************************************************************************************************/

void BladeCountB() {
  if(digitalRead(B)==HIGH) {     //B가 Rising 할 때
    if(digitalRead(A)==HIGH) {     //A가 Rising 한다면
    pos ++;     //시계방향으로 돌고
    rotateDirec="clockwise";     //"clockwise" 출력
    }else{     //B가 Falling한다면
    pos --;     //반시계 방향으로 돌고
    rotateDirec="counterclockwise";     //"counterclockwise" 출력
    }
  }
  else{     //B가 Falling 할 때
    if(digitalRead(A)==LOW) {     //A가 Falling 한다면
    pos ++;     //시계 방향으로 돌고
    rotateDirec="clockwise";     //"clockwise" 출력
    }else{     //A가 Rising 한다면
    pos --;     //반시계 방향으로 돌고
    rotateDirec="counterclockwise";     //"counterclockwise" 출력
    }
  }
}

/*******************************************************************************/

void loop() {
  Serial.print("방향: ");
  Serial.print(rotateDirec);     //회전 방향 출력
  Serial.print("\t");
  Serial.print("위치");
  Serial.println(pos);     //위치 출력
}
