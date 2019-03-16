/*
  Código Realizado por: Grupo de Laboratorio 1
  Título: 1. LED parpadeante (Adicional LED 13)
  Asignatura: Periféricos
      USAL 2018 - 2019
*/

int salidaLED = 0; // LED APAGADO = 0, LED ENCENDIDO = 1

void setup() 
{
  pinMode(4, OUTPUT); // Declarar el LED como salida en el pin 4
  pinMode(13, OUTPUT); // Declarar el LED integrado en Arduino pin 13
}

void loop() 
{
  salidaLED = digitalRead(4); // Leer el estado del pin 4

  if (salidaLED == 0) // Comprobar estado LED ¿Apagado?
  {
    digitalWrite(13, HIGH); // Encender LED 13
    delay(5000); // Esperar 5 segundos
    digitalWrite(13, LOW); // Apagar LED 13
    digitalWrite(4, HIGH); // Encender LED pin 4
  }

  if(salidaLED == 1) // Comprobar estado LED ¿Encendido?
  {
    delay(5000); // Esperar 5 segundos
    digitalWrite(4, LOW); // Apagar LED pin 4
  }
}
