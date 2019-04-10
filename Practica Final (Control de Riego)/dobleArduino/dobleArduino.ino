
#include <SoftwareSerial.h>
int i=0;
SoftwareSerial Serie2(10,11);
void setup() 
{
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Serie2.begin(9600);
  digitalWrite(13,LOW);
}

void loop(){
  Serie2.write("r");
  delay(3000);
  Serie2.write("l");
  delay(3000);
}
void setup(){
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  while(Serial.available()){
    char dato= Serial.read();
    switch(dato){
      case 'r':
        digitalWrite(13,HIGH);
        delay(80);
        digitalWrite(13,LOW);
        delay(80);
        break;
      case 'l':
      digitalWrite(13,HIGH);
        delay(200);
        digitalWrite(13,LOW);
        delay(200);
        break;
    }
  
  }
}
