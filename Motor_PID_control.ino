#define A 20     
#define B 21    
#include <MsTimer2.h>     

int rotate_min = 95;     //Dead zone(you must measure it)
int rotate_max = 255;
int pulse_min = 9;     //pulse value when its PWM is rotate_min
int pulse_max = 64;     //pulse value when its PWM is rotate_max

int pos = 0;   
int old_pos = 0;     //previous position
int bottom_line = 0;     //for the serial plotter's visibility
double ppu = 0;     //pulse per unit time(20ms)
double ppu_old = 0;

double Kp = 15;     //P gain value(you must tune it)
double Ki = 2;     //I gain value(you must tune it)
double Kd = 6;     //D gain value(you must tune it)

double current_ppu = 0;     //current ppu
double desired_ppu = 0;     //desired ppu (you receive it from target value)
double old_desired_ppu = 0;     //previous desired ppu(used when measuring overshoot or undershoot)
double increased_ppu = 0;      //increased ppu
String Settle = "";     //X -> Settle X, O -> Settle O
double highest = 0;     //highest overshoot value
double lowest = 0;     //lowest undershoot value
double peak = 0;     //highest value of ppu
int tic = 0;     //start timer after serial starts(for measuring overshoot or undershoot value)
int SettleTime = 0;     //settle time(time taken to steady state)
int old_SettleTime = 0;     //previous settle time
int SettleTimer = 0;     //timer used for measuring settlement maintenance time

double target = 0;     //value that you put
double error = 0;     //error value
double error_old;     //previous error value
double P_control, I_control, D_control;
double PID_control;     
int PWM = 0;     //PWM value
int PWM_Input;     //restrained PWM value(0~255)


int clamping = 1;     //anti wind-up: Clamping 1 -> off, 0 -> on 
int saturation = 0;     //check motor saturation
int PID_Error = 1;     //verify that the error and controller output are the same sign


void setup() {

  pinMode(A, INPUT);    
  pinMode(B, INPUT);    

  pinMode(7, OUTPUT);    
  pinMode(8, OUTPUT);    
  
  attachInterrupt(3, BladeCountA, CHANGE);    
  attachInterrupt(2, BladeCountB, CHANGE);    
  Serial.begin(9600);     
  MsTimer2::set(20, PID);   
  MsTimer2::start();    
  
  Serial.println("desired,current");
}





void BladeCountA() {
  if (digitalRead(A) == HIGH) {  
    if (digitalRead(B) == LOW) {  
      pos ++;    
    } else {    
      pos --;    
    }
  }
  else { //A가 Falling할 때
    if (digitalRead(B) == HIGH) { 
      pos ++;    
    } else {   
      pos --;     
    }
  }
}




void BladeCountB() {
  if (digitalRead(B) == HIGH) {  
    if (digitalRead(A) == HIGH) {
      pos ++;     
    } else {    
      pos --;   
    }
  }
  else {    //B가 Falling 할 때
    if (digitalRead(A) == LOW) {  
      pos ++;     
    } else {  
      pos --;   
    }
  }
}



void PID() {
  tic ++;     //settle time
  ppu = pos - old_pos;    
  old_pos = pos;


  //Anti-Windup : Clamping
  if (PID_Error * saturation == 1) {
    clamping = 0;
  } else {
    clamping = 1;
  }

  //PID_Control
  error = desired_ppu - ppu;
  P_control = Kp * error;
  I_control += clamping * Ki * error;
  D_control = Kd * (error - error_old);
  error_old = error;
  PID_control = (P_control + I_control + D_control);
  
  //Clamping: verify that the error and controller output are the same sign
  if (PID_control * error > 0) {
    PID_Error = 1;
  } else {
    PID_Error = 0;
  }

  //PWM calculation
    if (desired_ppu >= 0) {
    PWM = rotate_min + (rotate_max - rotate_min) * PID_control / pulse_max;
    PWM_Input = constrain(PWM, 0, 255);
  } else if (desired_ppu < 0) {
    PWM = -rotate_min + (rotate_max - rotate_min) * PID_control / pulse_max;
    PWM_Input = constrain(PWM, -255, 0);
  }

  //Clamping: check motor saturation
  if (PWM != PWM_Input) {
    saturation = 1;
  } else {
    saturation = 0;
  }

  //Motor operation
  if (target > 0) {
    digitalWrite(7, HIGH);     
    digitalWrite(8, LOW);
    analogWrite(6, PWM_Input);

  }
  else if (target < 0) {
    digitalWrite(7, LOW);     
    digitalWrite(8, HIGH);
    analogWrite(6, -PWM_Input);
  }
  else{
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    analogWrite(6, 0);
  }
 

  //+settle time
  increased_ppu = ppu - ppu_old;     //ppu gradient
  if (abs(increased_ppu) == 0); {
    peak = ppu;     //local extremum value of ppu
  }

  if ((abs(desired_ppu - peak)) <= (abs(desired_ppu) / 20)) {     //Ensure that it falls within 5% of the final value
    Settle = "O";
  }
  else {
    Settle = "X";
  }

   ppu_old = ppu;

  if (Settle == "O") {
    SettleTimer ++;
  }
  if (Settle == "X") {
    SettleTimer = 0;
  }

  if (SettleTimer >= 25) {     //If the settling condition persists for 0.5 seconds
    SettleTime = tic - 25;
  }

  if ((old_SettleTime > 0) && (old_SettleTime < SettleTime)) { //Stop Setletime update if it enters a settlement state
    SettleTime = old_SettleTime;
  }

  old_SettleTime = SettleTime;

  
}


 


void overshooting() {
  if (desired_ppu > old_desired_ppu) {    //When the target is greater than the past target
    if (ppu > highest) {     //If the current speed is greater than the previous maximum,
      highest = ppu;     //Renew the maximum value of overshoot
      
    }
  } else if (desired_ppu < old_desired_ppu) {     //When the target is less than the past target
    if (ppu < lowest) {     //If the current speed is less than the previous minimum,
      lowest = ppu;     //Renew the minimum value of undershoot
    }
  }
}



void loop() {
  if (Serial.available()) {    //Run if a serial signal is received
    target = Serial.parseInt();
    if (Serial.read() == '\n') {
      tic = 0;     //initialize time
      SettleTime = 0;     //initialize
      highest = ppu;     //initialize overshoot
      lowest = ppu;     //initialize undershoot
      old_desired_ppu = desired_ppu;
      desired_ppu = target;
    }
  }
  overshooting();

  Serial.print("desired ppu:");
  Serial.print(desired_ppu);
  Serial.print("  ");
  Serial.print("current ppu:");
  Serial.print(ppu);
  Serial.print("  ");
  Serial.print("pos:");
  Serial.print(pos);
  Serial.print("  ");
  Serial.print("highest:");
  Serial.print(highest);
  Serial.print("  ");
  Serial.print("lowest:");
  Serial.print(lowest);
  Serial.print("  ");
  Serial.print("Settle:");
  Serial.print(Settle);     //X -> Settle X, O -> Settle O
  Serial.print("  ");
  Serial.print("SettleTime:");
  Serial.print(SettleTime);     //안정 상태까지 걸린 시간
  Serial.println(bottom_line);     //graph용
  
}
