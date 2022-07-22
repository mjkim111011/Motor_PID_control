int in1=7;
int in2=8;
int in_data;
String a="clockwise";
String b="countclockwise";
String c="stop";

void setup() {
pinMode(in1,OUTPUT); 
pinMode(in2,OUTPUT);
Serial.begin(19200);
}

void loop() {
  if(Serial.available())
    {
    in_data = Serial.read();
     {
     if(Serial.read()=='\n') {
     }  
     }

     {
      if(in_data == '1')
      {
       digitalWrite(in1, HIGH);
       digitalWrite(in2, LOW);
       analogWrite(6, 255);
       Serial.println(a);
      }
      
      else if(in_data == '2')
      {
       digitalWrite(in1, LOW);
       digitalWrite(in2, HIGH);
       analogWrite(6, 255);
       Serial.println(b);
      }
      
      else if (in_data == '0')
      {
       digitalWrite(in1, LOW);
       digitalWrite(in2, LOW);
       Serial.println(c);
      }
     }
    }
  }
