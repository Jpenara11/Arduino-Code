/*
  Código Realizado por: Grupo de Laboratorio 1
  Título: 7. Estudio Funcionamiento Microservos
  Asignatura: Periféricos
      USAL 2018 - 2019
*/

#include <Servo.h>

const int EntradaServo = 6; // Declaramos pin del Servo
Servo servo; // Declaramos el objeto servo

void setup() 
{
  Serial.begin(9600);
  pinMode(EntradaServo, OUTPUT);
  servo.attach(EntradaServo); // Vinculas el servo al pin 9
  servo.write(0);// Ponemos a 0 grados
  delay(1000);// Esperamos 1 segundo
}

void loop() 
{


  servo.write(15);
  delay(500);// Incrementamos en 15 grados el eje del servo
  servo.write(30);
    delay(500);// Incrementamos en 30 grados el eje del servo
 servo.write(45);
   delay(500);// Incrementamos en 45 grados el eje del servo
  servo.write(60);
    delay(500);// Incrementamos en 60 grados el eje del servo
 servo.write(75);
   delay(500);// Incrementamos en 75 grados el eje del servo
  servo.write(90);
    delay(1000);// Incrementamos en 90 grados el eje del servo
   servo.write(0);
   delay(500);
}
