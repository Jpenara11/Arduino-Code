/*
  Código Realizado por: Grupo de Laboratorio 1
  Título: 7. Estudio Funcionamiento Microservos
  Asignatura: Periféricos
      USAL 2018 - 2019
*/

#include <Servo.h>

const int EntradaServo = 9; // Declaramos pin del Servo
const int LDR = A0; // Declaramos entrada LDR
int valorLDR = 0; // Variable donde se almacena el valor leido de la LDR
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
  valorLDR = analogRead(LDR); // Leemos el valor analogico de la LDR

  Serial.println(valorLDR); // Medimos el valor de la LDR para regular los valores que puede alcanzar la LDR

  if(valorLDR >= 780 && valorLDR <= 810)  servo.write(15); // Incrementamos en 15 grados el eje del servo

  if(valorLDR > 810 && valorLDR <= 840)  servo.write(30); // Incrementamos en 30 grados el eje del servo

  if(valorLDR > 840 && valorLDR <= 870) servo.write(45); // Incrementamos en 45 grados el eje del servo

  if(valorLDR > 870 && valorLDR <= 900) servo.write(60); // Incrementamos en 60 grados el eje del servo

  if(valorLDR > 900 && valorLDR <= 930) servo.write(75); // Incrementamos en 75 grados el eje del servo

  if(valorLDR > 930 && valorLDR <= 970) servo.write(90); // Incrementamos en 90 grados el eje del servo

}
