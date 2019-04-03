const int flexPin=A0;
int val=0;
void setup(){

Serial.beging(9600);
}
void loop(){

val=analogRead(flexPin);
int angle=map(val,32,85,0,90);
Serial.println(String(angle));
delay(500);
}